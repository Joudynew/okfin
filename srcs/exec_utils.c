/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 21:30:00 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

void	cleanup_heredocs(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->redir_type == HEREDOC && node->heredoc_tmpfile)
	{
		unlink(node->heredoc_tmpfile);
		free(node->heredoc_tmpfile);
		node->heredoc_tmpfile = NULL;
	}
	cleanup_heredocs(node->left);
	cleanup_heredocs(node->right);
	cleanup_heredocs(node->redirections);
}
