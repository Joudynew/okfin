/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:43:57 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/23 01:46:49 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env_node(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_or_replace(t_env **env, char *cmd)
{
	t_env	*found;
	char	*name;
	char	*value;
	int		i;

	i = 0;
	value = NULL;
	while (cmd[i] && cmd[i] != '=')
		i++;
	name = ft_substr(cmd, 0, i);
	if (!name)
		return ;
	if (cmd[i] == '=')
		value = ft_strdup(&cmd[i + 1]);
	found = find_env_node(*env, name);
	if (found)
	{
		free(found->value);
		found->value = value;
		return (free(name));
	}
	else
		add_env_node(env, name, value);
	free(name);
	free(value);
}

void	add_export_only(t_env **env, char *cmd)
{
	t_env	*found;
	char	*name;

	name = ft_strdup(cmd);
	if (!name)
		return ;
	found = find_env_node(*env, name);
	if (!found)
		add_env_node(env, name, NULL);
	free(name);
}
