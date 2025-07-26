/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:42:13 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 18:40:31 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_main_data(t_main_data *data, char **envp)
{
	data->input = NULL;
	data->expanded = NULL;
	data->tokens = NULL;
	data->tokens_head = NULL;
	data->ast = NULL;
	data->env_list = create_env_list(envp);
	data->exit_status = 0;
	data->envp = envp;
}

void	reset_loop_data(t_main_data *data)
{
	data->input = NULL;
	data->expanded = NULL;
	data->tokens = NULL;
	data->tokens_head = NULL;
	data->ast = NULL;
}

void	cleanup_shell(t_token *tokens, t_ast_node *ast, char *input)
{
	free_tokens(&tokens);
	free_ast(ast);
	free(input);
}

void	cleanup_final(t_main_data *data)
{
	cleanup_shell(data->tokens_head, data->ast, data->input);
	clear_history();
	rl_clear_history();
	free_list(data->env_list);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '&' || c == ';' || c == '>'
		|| c == '<' || c == '\'' || c == '\"');
}
