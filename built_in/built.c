/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:39:23 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 21:39:24 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	builtin_pwd(void)
{
	char	*cwd;

	cwd = (char *)malloc(1024 * sizeof(char));
	if (cwd == NULL)
	{
		perror("malloc");
		return (1);
	}
	if (getcwd(cwd, 1024) == NULL)
	{
		perror("getcwd");
		free(cwd);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}

int	built(t_command *cmd, t_env **env, t_shell *minishell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd, (*env), minishell));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		if (cmd->args[1] && cmd->args[2])
		{
			ft_putendl_fd("minishell: cd: too many arguments", 2);
			minishell->last_status = 1;
			return (-1);
		}
		return (builtin_cd(cmd->args, minishell));
	}
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env((*env)));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd->args, env, minishell));
	if (ft_strcmp(cmd->args[0], "unset") == 0 && cmd->args[1])
		return (builtin_unset(cmd->args[1], env));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	return (1);
}

int	is_builtin(char **cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd[0], "cd") || !ft_strcmp(cmd[0], "echo")
		|| !ft_strcmp(cmd[0], "exit") || !ft_strcmp(cmd[0], "pwd")
		|| !ft_strcmp(cmd[0], "env") || !ft_strcmp(cmd[0], "export")
		|| !ft_strcmp(cmd[0], "unset"));
}
