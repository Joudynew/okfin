/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:39:27 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 16:39:28 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_exp(char *cmd)
{
	int	i;

	if (!cmd || !cmd[0])
		return (0);
	if (!ft_isalpha(cmd[0]) && cmd[0] != '_')
	{
		ft_putstr_fd("minishell: export: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier2\n", STDERR_FILENO);
		return (0);
	}
	i = 1;
	while (cmd[i] && cmd[i] != '=')
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
		{
			ft_putstr_fd("minishell: export: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

void	print_export(char **env_cpy)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env_cpy[i])
	{
		ft_putstr_fd("declare -x ", 1);
		while (env_cpy[i][j] && env_cpy[i][j] != '=')
		{
			write(1, &env_cpy[i][j], 1);
			j++;
		}
		if (env_cpy[i][j] == '=')
		{
			write(1, "=", 1);
			write(1, "\"", 1);
			ft_putstr_fd(&env_cpy[i][j + 1], 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
		j = 0;
	}
}

char	**cpy_env_to_tab(t_env *env, int i, int count)
{
	char	**tab;

	(void)i;
	(void)count;
	count = count_env_nodes(env);
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (NULL);
	fill_env_tab(env, tab);
	return (tab);
}

int	handle_export_no_args(t_env *env_list)
{
	int		len;
	char	**cpy_env;

	len = 0;
	cpy_env = cpy_env_to_tab(env_list, 0, 0);
	while (cpy_env[len])
		len++;
	export_sorted(cpy_env, len);
	print_export(cpy_env);
	free_split(cpy_env);
	return (1);
}

int	builtin_export(t_env *env_list, char **cmd)
{
	int	i;

	i = 1;
	if (!cmd[1])
		return (handle_export_no_args(env_list));
	else
	{
		if (is_valid_exp(cmd[i]))
		{
			if (ft_strchr(cmd[i], '='))
				add_or_replace(&env_list, cmd[i]);
			else
				add_export_only(&env_list, cmd[i]);
		}
	}
	return (1);
}
