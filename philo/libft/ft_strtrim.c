/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vburton < vburton@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 19:39:35 by vburton           #+#    #+#             */
/*   Updated: 2023/01/26 17:25:49 by vburton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_start(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i])
	{
		if ((ft_strchr(set, (int)s1[i])) == 0)
			break ;
		i++;
	}
	return (i);
}

size_t	ft_end(char const *s1, char const *set)
{
	size_t	j;

	j = ft_strlen((char *)s1);
	while (j > 0)
	{
		if ((ft_strchr(set, (int)s1[j])) == 0)
			break ;
		j--;
	}
	return (j);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*res;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	i = ft_start(s1, set);
	j = ft_end(s1, set);
	len = j - i + 1;
	if (i > j)
		return (ft_strdup(""));
	res = ft_substr(s1, i, len);
	return (res);
}
