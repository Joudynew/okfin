/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:40:05 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:06:41 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	free_in_child(t_env *env_list, t_ast_node *node, char *path)
{
	free_list(env_list);
	free_ast(node);
	if (path)
		free(path);
}

int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	needs_child_process(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "env") == 0);
}

void	handle_signal_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		else if (sig == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
	}
}
