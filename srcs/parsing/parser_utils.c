/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 20:56:02 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/24 18:38:17 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast_node	*create_ast_node(t_node_type node_type)
{
	t_ast_node	*new_node;

	new_node = malloc(sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = node_type;
	new_node->args = NULL;
	new_node->args_count = 0;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->redirections = NULL;
	new_node->filename = NULL;
	new_node->heredoc_tmpfile = NULL;
	new_node->redir_type = 0;
	return (new_node);
}

t_ast_node	*create_redir_node(t_ast_node *cmd, t_token *token_list)
{
	t_ast_node		*redir;
	t_token_type	redir_type;

	(void)cmd;
	redir = NULL;
	redir_type = (token_list)->type;
	token_list = (token_list)->next;
	if (!token_list || (token_list)->type != WORD)
	{
		write(STDERR_FILENO, "syntax error\n", 13);
		free_ast(cmd);
		return (NULL);
	}
	redir = create_ast_node(NODE_REDIRECT);
	redir->redir_type = redir_type;
	redir->filename = ft_strdup((token_list)->value);
	return (redir);
}

int	add_args_to_cmd(t_ast_node *cmd, char *arg)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = malloc(sizeof(char *) * (cmd->args_count + 2));
	if (!tmp)
		return (0);
	while (i < cmd->args_count)
	{
		tmp[i] = cmd->args[i];
		i++;
	}
	tmp[cmd->args_count] = arg;
	tmp[cmd->args_count + 1] = NULL;
	free(cmd->args);
	cmd->args = tmp;
	cmd->args_count++;
	return (1);
}

void	add_redir_to_cmd(t_ast_node *cmd, t_ast_node *redir)
{
	t_ast_node	*current;

	if (!cmd || !redir)
		return ;
	if (!cmd->redirections)
		cmd->redirections = redir;
	else
	{
		current = cmd->redirections;
		while (current->right)
			current = current->right;
		current->right = redir;
	}
}

void	free_ast(t_ast_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->args)
	{
		while (i < node->args_count)
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->filename)
		free(node->filename);
	if (node->heredoc_tmpfile)
		free(node->heredoc_tmpfile);
	if (node->redirections)
		free_ast(node->redirections);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}
