/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:54:47 by ktoraman          #+#    #+#             */
/*   Updated: 2024/10/25 16:49:55 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	count;
	size_t	size;
	char	*tab;

	if (!s)
		return (NULL);
	if ((unsigned int)ft_strlen(s) < start)
		return (ft_strdup(""));
	size = ft_strlen(s + start);
	if (size < len)
		len = size;
	tab = (char *)malloc((len + 1) * sizeof(char));
	if (!tab)
		return (NULL);
	count = 0;
	while (count < len)
	{
		tab[count] = s[start + count];
		count++;
	}
	tab[count] = '\0';
	return (tab);
}
