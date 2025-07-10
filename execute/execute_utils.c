/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:20:51 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/10 15:03:49 by ktoraman         ###   ########.fr       */
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

void check_permissions_exec(const char *path, t_command *cmd, char **env)
{
	struct stat st;

	if (!path || path[0] == '\0')
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		free_commands(cmd);
		ft_free_tab(env);
		exit(127);
	}
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free_commands(cmd);
		ft_free_tab(env);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) && ft_strchr(path, '/'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putendl_fd(": Is a directory", 2);
		free_commands(cmd);
		ft_free_tab(env);
		exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		if (access(path, X_OK) != 0)
    	{
        	ft_putstr_fd("minishell: ", 2);
        	ft_putstr_fd((char *)path, 2);
        	if (ft_strchr(path, '/'))
        		ft_putendl_fd(": Permission denied", 2), exit(126);
        	else
        	ft_putendl_fd(": command not found", 2), exit(127);
    	}
		free_commands(cmd);
		ft_free_tab(env);
		exit(126);
	}
}
