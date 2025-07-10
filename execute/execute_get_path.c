/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:07:23 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/10 14:57:04 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	**get_path_dirs(char **env)
{
	while (*env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (!*env)
		return (NULL);
	return (ft_split(*env + 5, ':'));
}

static char	*find_cmd_in_dirs(char **dirs, char *cmd)
{
	int			idx;
	char		*path;
	char		*tmp;

	idx = 0;
	while (dirs && dirs[idx])
	{
		tmp = ft_strjoin(dirs[idx], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
		idx++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	**dirs;
	char	*path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	dirs = get_path_dirs(env);
	if (!dirs)
		return (NULL);
	path = find_cmd_in_dirs(dirs, cmd);
	ft_free_tab(dirs);
	return (path);
}
