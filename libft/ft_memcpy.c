/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:37:34 by ysoyturk          #+#    #+#             */
/*   Updated: 2024/11/01 17:55:39 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*ptr1;
	const unsigned char	*ptr2;

	if (!dst && !src)
		return (NULL);
	ptr1 = (unsigned char *)dst;
	ptr2 = (unsigned char *)src;
	while (n-- > 0)
		*(ptr1++) = *(ptr2++);
	return (dst);
}
