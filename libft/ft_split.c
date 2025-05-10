/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:09:40 by ysoyturk          #+#    #+#             */
/*   Updated: 2024/11/01 17:25:24 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char sep)
{
	int	count_word;

	count_word = 0;
	while (*s)
	{
		while (*s == sep)
			s++;
		if (*s)
			++count_word;
		while (*s && *s != sep)
			++s;
	}
	return (count_word);
}

static char	**free_all(char **res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	int		j;
	int		k;
	char	**res;

	res = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!res || !s)
		return (NULL);
	k = 0;
	while (*s)
	{
		while (*s && *s == c)
			++s;
		if (*s && *s != c)
		{
			j = 0;
			while (s[j] && s[j] != c)
				j++;
			res[k] = ft_substr(s, 0, j);
			if (res[k++] == NULL)
				return (free_all(res));
			s += j;
		}
	}
	res[k] = NULL;
	return (res);
}
