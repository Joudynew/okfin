/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:32:50 by joudafke          #+#    #+#             */
/*   Updated: 2024/11/22 00:31:26 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, int n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n)
	{
		*str = (unsigned char)c;
		str++;
		n--;
	}
	return (s);
}
/*
#include <stdio.h>
int	main()
{
	char str[50] = "coucou joujou";
	ft_memset(str, 'A', 5);
	printf("%s\n", str);
	return (0);
}
*/
