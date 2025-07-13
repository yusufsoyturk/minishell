/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:20:51 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 10:15:05 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_permissions(const char *path)
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

static void	exec_perm_error(char *path, t_command *cmd, char **env, int code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	if (code == 126)
		ft_putendl_fd(": Permission denied", 2);
	else
		ft_putendl_fd(": command not found", 2);
	free_commands(cmd);
	ft_free_tab(env);
	free(path);
	exit(code);
}

static void	exec_isdir_error(char *path, t_command *cmd, char **env)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)path, 2);
	ft_putendl_fd(": Is a directory", 2);
	free_commands(cmd);
	free(path);
	ft_free_tab(env);
	exit(126);
}

static void	exec_nf_error(char *msg, t_command *cmd, char **env, int code)
{
	ft_putstr_fd(msg, 2);
	free_commands(cmd);
	ft_free_tab(env);
	exit(code);
}

void	check_permissions_exec(char *path, t_command *cmd, char **env)
{
	struct stat	st;

	if (!path || path[0] == '\0')
		exec_nf_error("minishell: command not found\n", cmd, env, 127);
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free_commands(cmd);
		free(path);
		ft_free_tab(env);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) && ft_strchr(path, '/'))
		exec_isdir_error(path, cmd, env);
	if (access(path, X_OK) != 0)
	{
		if (ft_strchr(path, '/'))
			exec_perm_error(path, cmd, env, 126);
		exec_perm_error(path, cmd, env, 127);
	}
}
