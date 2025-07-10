/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utilss.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:07:23 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/10 14:21:55 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char *get_path(char *cmd, char **env)
{
	int i = 0;
	char **res;
	char *path, *path_slash;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (*env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (!*env)
		return NULL;
	res = ft_split(*env + 5, ':');
	if (!res)
		return NULL;
	while (res[i])
	{
		path_slash = ft_strjoin(res[i], "/");
		path = ft_strjoin(path_slash, cmd);
		free(path_slash);
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_free_tab(res);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_tab(res);
	return (NULL);
}
