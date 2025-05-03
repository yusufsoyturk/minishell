/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:55:23 by ktoraman          #+#    #+#             */
/*   Updated: 2024/10/22 19:50:40 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	ft_lennmb(int n, int sign)
{
	int	i;

	i = 0;
	if (n == 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	if (sign < 0)
		i++;
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	numb;
	int		sign;
	int		len;

	numb = n;
	sign = 1;
	if (n < 0)
		sign = -1;
	len = ft_lennmb(n, sign);
	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (sign == -1)
		numb *= -1;
	while (len > 0)
	{
		len--;
		str[len] = ((numb % 10) + 48);
		numb = numb / 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}
