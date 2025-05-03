/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:58:09 by ktoraman          #+#    #+#             */
/*   Updated: 2024/10/25 17:07:28 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	ft_striteri(char *s, void (*f)(unsigned int, char *))
// {
// 	unsigned int	i;

// 	if (!s)
// 		return (NULL);
// 	i = 0;
// 	while (s[i])
// 	{
// 		f(i, &s[i]);
// 		i++;
// 	}
// }
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, s + i);
		i++;
	}
}
