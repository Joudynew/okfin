/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:41:40 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:37:30 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == '&')
		return (1);
	return (0);
}

int	ending_quotes(char *input, int start)
{
	char	quote;
	int		i;

	quote = input[start];
	i = start + 1;
	while (input[i] && input[i] != quote)
		i++;
	return (i);
}

t_token	*create_token(t_token_type token_type, char *value)
{
	t_token	*new_token;
	int		i;

	i = 0;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = token_type;
	if (!value)
		new_token->value = NULL;
	else
	{
		while (value[i])
		{
			if (value[i] < 0)
				value[i] *= -1;
			i++;
		}
		new_token->value = ft_strdup(value);
	}
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **token_list, t_token *new_token)
{
	t_token	*tmp;

	if (!*token_list)
	{
		*token_list = new_token;
		return ;
	}
	tmp = *token_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

void	free_tokens(t_token **token_list)
{
	t_token	*tmp;
	t_token	*ok;

	ok = *token_list;
	while (ok)
	{
		tmp = ok;
		ok = ok->next;
		if (tmp->value)
		{
			free(tmp->value);
		}
		free(tmp);
	}
}
