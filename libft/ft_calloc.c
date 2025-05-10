/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:49:32 by ysoyturk          #+#    #+#             */
/*   Updated: 2024/11/01 17:57:00 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*array;

	array = (void *)malloc(num * size);
	if (array == NULL)
		return (NULL);
	ft_bzero(array, (num * size));
	return (array);
}
