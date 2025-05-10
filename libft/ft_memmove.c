/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:47:10 by ysoyturk          #+#    #+#             */
/*   Updated: 2024/11/01 17:55:36 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*ptr1;
	const unsigned char	*ptr2;

	ptr1 = (unsigned char *)dst;
	ptr2 = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	if (ptr2 < ptr1)
	{
		while (len-- > 0)
			ptr1[len] = ptr2[len];
	}
	else
	{
		while (len-- > 0)
			*(ptr1++) = *(ptr2++);
	}
	return (dst);
}
