/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:20:51 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/07 17:21:24 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int check_permissions(const char *path)
{
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (-1);
	}
	return (0);
}
