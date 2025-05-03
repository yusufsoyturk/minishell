/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:03:48 by ktoraman          #+#    #+#             */
/*   Updated: 2024/10/25 17:03:52 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*src1;
	char			*src2;
	char			*ptr;
	unsigned int	i;
	unsigned int	j;

	ptr = (char *)malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (!ptr)
		return (NULL);
	src1 = (char *)s1;
	src2 = (char *)s2;
	i = 0;
	j = 0;
	while (src1[i])
	{
		ptr[i] = src1[i];
		i++;
	}
	while (src2[j])
	{
		ptr[i + j] = src2[j];
		j++;
	}
	ptr[i + j] = '\0';
	return (ptr);
}
