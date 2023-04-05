/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton < vburton@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 19:03:34 by vburton           #+#    #+#             */
/*   Updated: 2023/01/25 15:05:15 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	size_tot;
	char	*res;

	i = -1;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		return ((char *)s1);
	if (!s1)
		return ((char *)s2);
	size_tot = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	if (size_tot >= SIZE_MAX)
		return (NULL);
	res = malloc(sizeof(char) * (size_tot + 1));
	if (!res)
		return (NULL);
	while (s1[++i])
		res[i] = s1[i];
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}
