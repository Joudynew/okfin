/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:18:35 by joudafke          #+#    #+#             */
/*   Updated: 2024/11/13 19:50:51 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		i;

	i = ft_strlen(str);
	while (str[i] != (unsigned char)c)
	{
		if (i == 0 && str[i] != c)
			return ((char *) 0);
		i--;
	}
	return ((char *) &str[i]);
}
