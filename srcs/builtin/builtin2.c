/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:42:46 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_builtin(t_builtin_data *data)
{
	t_exit_data	exit_data;

	exit_data.env_list = data->env_list;
	exit_data.node = data->node;
	exit_data.input = data->input;
	exit_data.token = data->token;
	exit_data.input_for_free = data->input_for_free;
	builtin_exit(&exit_data);
}

void	exec_builtin_commands(t_builtin_data *data)
{
	if (ft_strncmp(data->node->args[0], "pwd", 4) == 0)
		builtin_pwd(data->node->args);
	else if (ft_strncmp(data->node->args[0], "env", 4) == 0)
		builtin_env(data->env_list);
	else if (ft_strncmp(data->node->args[0], "echo", 5) == 0)
		builtin_echo(data->node->args);
	else if (ft_strncmp(data->node->args[0], "cd", 3) == 0)
		ft_cd(data->node->args[1]);
	else if (ft_strncmp(data->node->args[0], "export", 7) == 0)
		builtin_export(data->env_list, data->node->args);
	else if (ft_strncmp(data->node->args[0], "unset", 6) == 0)
		builtin_unset(data->env_list, data->node->args[1]);
	else if (ft_strncmp(data->node->args[0], "exit", 5) == 0)
		handle_exit_builtin(data);
}

void	exec_builtin(t_builtin_data *builtin_data)
{
	if (!builtin_data->node->args || !builtin_data->node->args[0])
		return ;
	exec_builtin_commands(builtin_data);
}
