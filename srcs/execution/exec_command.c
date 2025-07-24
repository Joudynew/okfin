/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 21:30:00 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/24 18:50:24 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_builtin_in_child(t_exec_data *data)
{
	t_builtin_data	builtin_data;

	builtin_data.node = data->node;
	builtin_data.env_list = data->env_list;
	builtin_data.input = NULL;
	builtin_data.token = data->token;
	builtin_data.input_for_free = data->input;
	exec_builtin(&builtin_data);
	free_in_child(data->env_list, data->first_node, NULL);
	free_tokens(&data->token);
	free(data->input);
	exit(0);
}

void	execute_external_command(t_exec_data *data)
{
	char	*path;

	path = get_cmd(data->env_list, data->node->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putendl_fd(data->node->args[0], STDERR_FILENO);
		free_in_child(data->env_list, data->first_node, NULL);
		free_tokens(&data->token);
		free(data->input);
		exit(127);
	}
	execve(path, data->node->args, data->envp);
	free_in_child(data->env_list, data->first_node, path);
	perror("execve");
	free_tokens(&data->token);
	free(data->input);
	exit(EXIT_FAILURE);
}

void	execute_child_command(t_exec_data *data)
{
	process_redirections(data->node->redirections);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (data->node->args && data->node->args[0]
		&& is_builtin(data->node->args[0])
		&& needs_child_process(data->node->args[0]))
		execute_builtin_in_child(data);
	else
		execute_external_command(data);
}

void	execute_builtin_main(t_ast_node *node, t_exec_data *data)
{
	t_builtin_data	builtin_data;

	builtin_data.node = node;
	builtin_data.env_list = data->env_list;
	builtin_data.input = NULL;
	builtin_data.token = data->token;
	builtin_data.input_for_free = data->input;
	exec_builtin(&builtin_data);
	cleanup_heredocs(node->redirections);
}

int	execute_command(t_ast_node *node, t_exec_data *data)
{
	pid_t	pid_cmd;
	int		status;

	if ((!node->args || !node->args[0]) && node->redirections)
		return (no_cmd_process_redirections(node->redirections), 0);
	if (node->args && node->args[0] && is_builtin(node->args[0])
		&& !needs_child_process(node->args[0]))
	{
		execute_builtin_main(node, data);
		return (0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (!data->is_pipe)
		pid_cmd = fork();
	if (!data->is_pipe && pid_cmd == -1)
		return (perror("fork"), EXIT_FAILURE);
	if (data->is_pipe || pid_cmd == 0)
		execute_child_command(data);
	else if (!data->is_pipe)
		waitpid(pid_cmd, &status, 0);
	handle_signal_status(status);
	cleanup_heredocs(node->redirections);
	return (0);
}
