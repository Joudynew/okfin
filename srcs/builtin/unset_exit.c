/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:44:46 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/23 01:22:14 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_cleanup_and_exit(t_exit_data *data, long exit_code)
{
	free_ast(data->node);
	free_list(data->env_list);
	free_tokens(&data->token);
	free(data->input_for_free);
	exit(exit_code);
}

void	builtin_exit(t_exit_data *data)
{
	long	exit_code;

	if (!data->node->args || !data->node->args[1])
	{
		exit_cleanup_and_exit(data, 0);
	}
	if (!ft_is_num(data->node->args[1]))
		handle_non_numeric_exit_arg(data->env_list, data->node->args,
			data->input);
	if (data->node->args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return ;
	}
	exit_code = ft_valid_nbr(data->node->args[1]) % 256;
	if (exit_code < 0)
		exit_code += 256;
	exit_cleanup_and_exit(data, exit_code);
}

t_env	*remove_env_node(t_env *env, t_env *previous, t_env *current)
{
	if (previous)
		previous->next = current->next;
	else
		env = current->next;
	free(current->name);
	free(current->value);
	free(current);
	return (env);
}

t_env	*builtin_unset(t_env *env, char *cmd)
{
	t_env	*previous;
	t_env	*current;

	previous = NULL;
	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, cmd) == 0)
		{
			return (remove_env_node(env, previous, current));
		}
		previous = current;
		current = current->next;
	}
	return (env);
}
