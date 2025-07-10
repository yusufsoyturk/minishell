/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:20:51 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/10 13:13:57 by ktoraman         ###   ########.fr       */
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

char **env_to_envp_array(t_env *env_list)
{
    int     count = 0;
    t_env   *curr = env_list;
    char    **envp;
    char    *tmp1;
    char    *tmp2;
    int     i = 0;
    int     j;

    while (curr)
    {
        if (curr->key)
            count++;
        curr = curr->next;
    }
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
	return NULL;
    j = 0;
    while (j <= count)
	envp[j++] = NULL;
    curr = env_list;
    while (curr)
    {
		if (curr->key)
        {
			tmp1 = ft_strjoin(curr->key, "=");
            if (!tmp1)
            {
                ft_free_tab(envp);
                return NULL;
            }
            if (curr->value)
            {
                tmp2 = ft_strjoin(tmp1, curr->value);
                free(tmp1);
                if (!tmp2)
                {
                    ft_free_tab(envp);
                    return NULL;
                }
                envp[i++] = tmp2;
            }
            else
            {
                envp[i++] = tmp1;
            }
        }
        curr = curr->next;
    }
    envp[i] = NULL;
    return envp;
}
