/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:14:05 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/05/12 15:01:03 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	size;
	char	*dst;

	size = ft_strlen(src);
	dst = (char *)malloc(size * sizeof(char) + 1);
	if (dst == NULL)
		return (0);
	ft_memcpy(dst, src, size);
	dst[size] = '\0';
	return (dst);
}
