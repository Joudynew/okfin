/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:00:47 by joudafke          #+#    #+#             */
/*   Updated: 2024/11/18 14:08:28 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

char	**ft_split(const char *s, char c)
{
	char	**str;
	size_t	lenword;
	int		i;

	i = 0;
	str = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!s || !str)
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			if (!ft_strchr(s, c))
				lenword = ft_strlen(s);
			else
				lenword = ft_strchr(s, c) - s;
			str[i++] = ft_substr(s, 0, lenword);
			s += lenword;
		}
	}
	str[i] = NULL;
	return (str);
}
/*
#include <stdio.h>
int	main()
{
	char	*st = "salut je suis joudy";
	char	s = ' ';
	char	**arr;

	arr = ft_split(st, s);
	for (int i = 0;  arr[i]; i++)
		printf("%s\n", arr[i]);
}
*/
