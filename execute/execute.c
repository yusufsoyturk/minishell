/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/07 19:46:38 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char *get_path(char *cmd, char **env)
{
	int i = 0;
	char **res;
	char *path, *path_slash;

	if (ft_strchr(cmd, '/'))
		return ft_strdup(cmd);

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
			return path;
		}
		free(path);
		i++;
	}
	ft_free_tab(res);
	return NULL;
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

static char *append_str(char *base, const char *suffix)
{
	char *new;
	if (base)
		new = ft_strjoin(base, suffix);
	else
		new = ft_strjoin("", suffix);
	free(base);
	return new;
}

char *expand_env_var_str(const char *str, t_env *env_list)
{
	char *result = NULL;
	char *tmp2;
	int  i = 0;

	while (str[i])
	{
		if (str[i] == '\'')
		{
			int start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			tmp2 = ft_substr(str, start, i - start);
			result = append_str(result, tmp2);
			free(tmp2);
			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '$')
		{
			if (str[i + 1] == '\0' || str[i + 1] == ' ' || str[i + 1] == '"')
			{
				tmp2 = ft_strdup("$");
				i++;
			}
			else
			{
				int start = ++i;
				while (ft_isalnum(str[i]) || str[i] == '_')
					i++;
				char *var_name = ft_substr(str, start, i - start);
				char *val = NULL;
				t_env *cur = env_list;
				while (cur)
				{
					if (ft_strcmp(cur->key, var_name) == 0)
					{
						val = ft_strdup(cur->value);
						break;
					}
					cur = cur->next;
				}
				free(var_name);
				if (val)
					tmp2 = val;
				else
					tmp2 = ft_strdup("");
			}
			result = append_str(result, tmp2);
			free(tmp2);
		}
		else
		{
			tmp2 = ft_substr(str, i, 1);
			result = append_str(result, tmp2);
			free(tmp2);
			i++;
		}
	}
	if (!result)
		result = ft_strdup("");
	return result;
}

char *expand_string(const char *input, t_env *env_list, int last_status)
{
	char *result = NULL;
	char *tmp2;
	int   i = 0;

	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
		{
			tmp2 = ft_itoa(last_status);
			result = append_str(result, tmp2);
			free(tmp2);
			i += 2;
		}
		else if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			int start = ++i;
			while (ft_isalnum(input[i]) || input[i] == '_')
				i++;
			char *var_name = ft_substr(input, start, i - start);
			char *value    = expand_env_var_str(var_name, env_list);
			free(var_name);
			result = append_str(result, value);
			free(value);
		}
		else
		{
			tmp2 = ft_substr(input, i, 1);
			result = append_str(result, tmp2);
			free(tmp2);
			i++;
		}
	}
	if (!result)
		result = ft_strdup("");
	return (result);
}

int execute(t_command *cmd, t_env **env_list, t_shell *mini)
{
	t_command	*current;
	pid_t		pid;
	int			pipe_fd[2];
	int			prev_fd;
	int			status;

	prev_fd = -1;
	status = 0;
	current = cmd;
	while (current)
	{
		mini->last_status = 0;
		if (handle_redirection(current, (*env_list), mini) < 0)
		{
			mini->last_status = 1;
			if (current->next)
			{
				if (pipe(pipe_fd) < 0)
					perror("pipe");
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
				current = current->next;
				mini->last_status = 0;
				continue ;
			}
			return mini->last_status;
		}
			if (!current->redirs && !current->next && is_builtin(current->args))
				return (built(current, env_list, mini));
			if (current->next && pipe(pipe_fd) < 0)
			return (perror("pipe"), 1);
			pid = fork();
			if (pid < 0)
				return (perror("fork"), 1);
			ignore_signals();
			if (pid == 0)
			{
				setup_child_signals();
				if (!current->args)
				{
					free_max(mini, *env_list, cmd);
					exit(0);
				}
				if (current->input != STDIN_FILENO)
					dup2(current->input, STDIN_FILENO);
				else if (prev_fd != -1)
					dup2(prev_fd, STDIN_FILENO);
				if (current->output != STDOUT_FILENO)
					dup2(current->output, STDOUT_FILENO);
				else if (current->next)
					dup2(pipe_fd[1], STDOUT_FILENO);
				if (current->next)
				{
					close(pipe_fd[0]);
					close(pipe_fd[1]);
				}
				if (is_builtin(current->args))
					exit(built(current, env_list, mini));
				if (!current->args[0] || current->args[0][0] == '\0')
				{
    				ft_putstr_fd("minishell: command not found\n", 2);
					free_env_list((*env_list));
					free_struct(mini);
					free_commands(cmd);
    				exit(127);
				}
				char **char_env;
				char_env = env_to_envp_array(*env_list);
				char *exec_path = get_path(current->args[0], char_env);
				if (!exec_path)
				{
					if (access(current->args[0], F_OK) == 0)
						exec_path = ft_strdup(current->args[0]);
					else	
					{
						ft_putstr_fd("minishell: ", 2);
						ft_putstr_fd(current->args[0], 2);
						ft_putendl_fd(": command not found", 2);
						free_env_list((*env_list));
						free_struct(mini);
						ft_free_tab(char_env);
						free_commands(cmd);
						exit(127);
					}
				}
				free_env_list((*env_list));
				free_struct(mini);
				check_permissions_exec(exec_path, cmd, char_env);
				execve(exec_path, current->args, char_env);
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->args[0], 2);
				ft_putendl_fd(": command not found", 2);
				free_commands(cmd);
				exit(127);
			}
			else
			{
				if (prev_fd != -1)
					close(prev_fd);
				if (current->next)
				{
					close(pipe_fd[1]);
					prev_fd = pipe_fd[0];
				}
			}
			current = current->next;
		}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
		write(1, "\n", 1);
	}
	setup_signals();
	if (WIFEXITED(status))
		mini->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->last_status = 128 + WTERMSIG(status);
	else 
		mini->last_status = 1;
	return (mini->last_status);
}
