/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:52:41 by joudafke          #+#    #+#             */
/*   Updated: 2024/11/16 14:36:33 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned int	s_len;
	unsigned int	i;

	i = 0;
	s_len = ft_strlen((char *)s);
	if (!s)
		return (NULL);
	if (start > s_len)
		return (ft_strdup(""));
	s = s + start;
	while (s[i] && len--)
		i++;
	substr = malloc(sizeof(char) * (i + 1));
	if (!substr)
		return (NULL);
	ft_strlcpy(substr, s, i + 1);
	return (substr);
}
