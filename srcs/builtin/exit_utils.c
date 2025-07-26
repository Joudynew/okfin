/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:38:25 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/26 16:38:26 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_valid_nbr(char *str)
{
	int			i;
	int			signe;
	long int	result;

	i = 0;
	signe = 1;
	result = 0;
	if (str[0] == '-' || str[0] == '+')
	{
		if (str[0] == '-')
			signe = -1;
		i++;
	}
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		result = result * 10 + (str[i] - 48);
		if ((result * signe) < INT_MIN || (result * signe) > INT_MAX)
			return (2);
		i++;
	}
	return (0);
}

void	cleanup(t_env *env_list, char **args, char *input)
{
	free_list(env_list);
	if (args)
		free_split(args);
	free(input);
	clear_history();
}

void	handle_non_numeric_exit_arg(t_env *env_list, char **args, char *input)
{
	ft_putstr_fd("exit: ", STDERR_FILENO);
	if (args[1])
		ft_putstr_fd(args[1], STDERR_FILENO);
	else
		ft_putstr_fd("null", STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	cleanup(env_list, args, input);
	exit(2);
}
