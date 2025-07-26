/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:39:08 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 16:39:09 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	fill_env_tab(t_env *env, char **tab)
{
	t_env	*tmp;
	char	*tmp_egal;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		if (tmp->value)
		{
			tmp_egal = ft_strjoin(tmp->name, "=");
			tab[i] = ft_strjoin(tmp_egal, tmp->value);
			free(tmp_egal);
		}
		else
			tab[i] = ft_strdup(tmp->name);
		i++;
		tmp = tmp->next;
	}
	tab[i] = NULL;
}

void	export_sorted(char **env_cpy, int len_env)
{
	int		i;
	int		sorted;
	char	*swp;

	sorted = 0;
	while (!sorted)
	{
		i = 0;
		sorted = 1;
		while (i < len_env - 1)
		{
			if (ft_strcmp(env_cpy[i], env_cpy[i + 1]) > 0)
			{
				swp = env_cpy[i + 1];
				env_cpy[i + 1] = env_cpy[i];
				env_cpy[i] = swp;
				sorted = 0;
			}
			i++;
		}
	}
}
