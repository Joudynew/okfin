/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:39:53 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:06:33 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	handle_pipe_child_left(t_ast_node *node, t_exec_data *data,
		int *pipe_fd)
{
	t_exec_data	child_data;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	child_data = *data;
	child_data.node = node->left;
	child_data.is_pipe = true;
	execute_ast(node->left, &child_data);
	free_in_child(data->env_list, data->first_node, NULL);
	free(data->input);
	free_tokens(&data->token);
	exit(EXIT_FAILURE);
}

void	handle_pipe_child_right(t_ast_node *node, t_exec_data *data,
		int *pipe_fd)
{
	t_exec_data	child_data;

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	child_data = *data;
	child_data.node = node->right;
	child_data.is_pipe = true;
	execute_ast(node->right, &child_data);
	free_in_child(data->env_list, data->first_node, NULL);
	free(data->input);
	free_tokens(&data->token);
	exit(EXIT_FAILURE);
}

int	execute_pipe(t_ast_node *node, t_exec_data *data)
{
	pid_t	pid_left;
	pid_t	pid_right;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), EXIT_FAILURE);
	pid_left = fork();
	if (pid_left == -1)
		return (perror("fork"), EXIT_FAILURE);
	if (pid_left == 0)
		handle_pipe_child_left(node, data, pipe_fd);
	pid_right = fork();
	if (pid_right == -1)
		return (perror("fork"), EXIT_FAILURE);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (pid_right == 0)
		handle_pipe_child_right(node, data, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, NULL, 0);
	return (0);
}
