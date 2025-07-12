/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:37:16 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 21:43:53 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	change_directory(char *target, t_shell *mini, char *cwd)
{
	if (chdir(target) == -1)
	{
		perror("cd");
		free(cwd);
		mini->last_status = 1;
		return (1);
	}
	return (0);
}

static void	update_pwd_env(char *cwd)
{
	char	*newpwd;

	setenv("OLDPWD", cwd, 1);
	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		setenv("PWD", newpwd, 1);
		free(newpwd);
	}
}

static int	cd_to_home(t_shell *mini, char *cwd)
{
	char	*target;

	target = getenv("HOME");
	if (!target)
	{
		free(cwd);
		return (1);
	}
	return (change_directory(target, mini, cwd));
}

static int	cd_to_oldpwd(t_shell *mini, char *cwd)
{
	char	*oldpwd;
	char	*target;

	oldpwd = getenv("OLDPWD");
	if (!oldpwd || ft_strcmp(oldpwd, cwd) == 0)
		target = "..";
	else
		target = oldpwd;
	if (change_directory(target, mini, cwd))
		return (1);
	if (ft_strcmp(target, "..") == 0)
	{
		target = getcwd(NULL, 0);
		if (target)
		{
			printf("%s\n", target);
			free(target);
		}
	}
	else
		printf("%s\n", oldpwd);
	return (0);
}

int	builtin_cd(char **args, t_shell *mini)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("getcwd"), 1);
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		if (cd_to_home(mini, cwd))
			return (1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (cd_to_oldpwd(mini, cwd))
			return (1);
	}
	else
	{
		if (change_directory(args[1], mini, cwd))
			return (1);
	}
	update_pwd_env(cwd);
	free(cwd);
	mini->last_status = 0;
	return (0);
}
