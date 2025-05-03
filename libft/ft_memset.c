/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:37:09 by ktoraman          #+#    #+#             */
/*   Updated: 2024/10/22 17:25:20 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int a, size_t c)
{
	size_t	i;

	i = 0;
	while (i < c)
	{
		((unsigned char *)b)[i] = (unsigned char)a;
		i++;
	}
	return (b);
}
