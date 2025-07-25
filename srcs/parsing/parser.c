/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:40:38 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:37:52 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast_node	*handle_word_token(t_token **token_list, t_ast_node *cmd)
{
	char	*tmp1;

	if (!cmd)
		cmd = create_ast_node(NODE_COMMAND);
	tmp1 = ft_strdup((*token_list)->value);
	add_args_to_cmd(cmd, tmp1);
	return (cmd);
}

t_ast_node	*handle_redir_token(t_token **token_list, t_ast_node *cmd)
{
	t_ast_node	*redir;

	if (!cmd)
		cmd = create_ast_node(NODE_COMMAND);
	redir = create_redir_node(cmd, *token_list);
	if (redir)
	{
		add_redir_to_cmd(cmd, redir);
		*token_list = (*token_list)->next;
		return (cmd);
	}
	else
		return (NULL);
}

t_ast_node	*parse_command(t_token **token_list, t_ast_node *cmd)
{
	if (!token_list || !*token_list)
		return (NULL);
	while (*token_list && (*token_list)->type != PIPE
		&& (*token_list)->type != EOF_TOKEN)
	{
		if ((*token_list)->type == WORD)
			cmd = handle_word_token(token_list, cmd);
		else if ((*token_list)->type == 2 || (*token_list)->type == 3
			|| (*token_list)->type == 4 || (*token_list)->type == 5)
		{
			cmd = handle_redir_token(token_list, cmd);
			if (!cmd)
				return (NULL);
		}
		*token_list = (*token_list)->next;
	}
	return (cmd);
}

t_ast_node	*handle_pipe_sequence(t_token **token_list, t_ast_node *left)
{
	t_ast_node	*right;
	t_ast_node	*node_pipe;

	*token_list = (*token_list)->next;
	right = parse_command(token_list, NULL);
	if (!right)
	{
		write(STDERR_FILENO, "syntax error\n", 13);
		free_ast(left);
		return (NULL);
	}
	node_pipe = create_ast_node(NODE_PIPE);
	node_pipe->left = left;
	node_pipe->right = right;
	return (node_pipe);
}

t_ast_node	*parse_pipeline(t_token **token_list)
{
	t_ast_node	*left;

	if (*token_list && (*token_list)->type == PIPE)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n", 50);
		return (NULL);
	}
	left = parse_command(token_list, NULL);
	if (!left)
		return (NULL);
	while (*token_list && (*token_list)->type == PIPE)
	{
		left = handle_pipe_sequence(token_list, left);
		if (!left)
			return (NULL);
	}
	if (*token_list && (*token_list)->type == EOF_TOKEN)
		*token_list = (*token_list)->next;
	return (left);
}
