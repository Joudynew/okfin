/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:15:31 by joudafke          #+#    #+#             */
/*   Updated: 2024/11/13 19:58:20 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		j;

	if (ft_strlen(little) < 1)
		return ((char *) big);
	i = 0;
	while (big[i] && i < len)
	{
		j = 0;
		while (little[j] == big[i + j] && i + j < len)
		{
			if (big[i + j] == little[j])
			{
				if (little[j + 1] == '\0')
					return ((char *) &big[i]);
			}
			else
				break ;
			j++;
		}
		i++;
	}
	return (0);
}
