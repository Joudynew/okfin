/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:00:00 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_rand(void)
{
	char	rand[32 + 1];

	create_rand(rand);
}

int	process_tokens_and_ast(t_main_data *data)
{
	data->expanded = expand_variables(data->input, data->env_list,
			data->exit_status);
	free(data->input);
	data->input = data->expanded;
	data->tokens = tokenize(data->input, 0);
	if (!data->tokens)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		free(data->input);
		return (1);
	}
	data->tokens_head = data->tokens;
	data->ast = parse_pipeline(&data->tokens);
	if (!data->ast)
	{
		cleanup_shell(data->tokens_head, data->ast, data->input);
		return (1);
	}
	return (0);
}

void	execute_main_loop(t_main_data *data)
{
	t_exec_data	exec_data;

	preprocess_all_heredocs(data->ast, data->env_list, &data->tokens_head,
		data->input);
	exec_data.node = data->ast;
	exec_data.envp = data->envp;
	exec_data.env_list = data->env_list;
	exec_data.token = data->tokens_head;
	exec_data.input = data->input;
	exec_data.first_node = data->ast;
	exec_data.is_pipe = false;
	execute_ast(data->ast, &exec_data);
	cleanup_heredocs(data->ast);
}

void	cleanup_final(t_main_data *data)
{
	cleanup_shell(data->tokens_head, data->ast, data->input);
	clear_history();
	rl_clear_history();
	free_list(data->env_list);
}
