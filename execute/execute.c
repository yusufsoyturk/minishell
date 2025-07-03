#include "../inc/minishell.h"

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

// Expand all $VARNAME in str, except inside single quotes
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

// Expand $? then $VARNAME in input
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
    return result;
}

// Handle a heredoc redirection: returns fd for reading; here_flag==0 expand, ==1 no expand
int handle_heredoc(t_redir *redir, t_env *env_list, t_shell *mini)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;

    if (pipe(pipefd) == -1)
        return (perror("pipe"), -1);
    pid = fork();
    if (pid == -1)
        return (perror("fork"), -1);

    ignore_signals();

    if (pid == 0)
    {
        char *raw;
        char *expanded;

        setup_child_signals();
        close(pipefd[0]);
        while (1)
        {
            raw = readline("> ");
            if (!raw)
                break;
            if (ft_strcmp(raw, redir->target) == 0)
            {
                free(raw);
                break;
            }
            if (redir->here_flag == 0)
                expanded = expand_string(raw, env_list, mini->last_status);
            else
                expanded = ft_strdup(raw);
            free(raw);
            ft_putendl_fd(expanded, pipefd[1]);
            free(expanded);
        }
        close(pipefd[1]);
        exit(0);
    }
    else
    {
        close(pipefd[1]);
        waitpid(pid, &status, 0);
        setup_signals();
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            write(1, "\n", 1);
            rl_replace_line("", 0);
            close(pipefd[0]);
            return (-1);
        }
        redir->fd = pipefd[0];
        return (pipefd[0]);
    }
}


int handle_redirection(t_command *cmd, t_env *env_list, t_shell *mini)
{
    int         fd;
    t_redir *r = cmd->redirs;

        while (r)
        {
            if (r->flag == R_HEREDOC)
                fd = handle_heredoc(r, env_list, mini);
            else
                fd = open(r->target, r->flag, 0644);
            if (fd < 0)
			{
				ft_putendl_fd(" No such file or directory", 2);
                return (-1);
			}

            r->fd = fd;
            if (r->flag == O_RDONLY || r->flag == R_HEREDOC)
                cmd->input = fd;
            else
                cmd->output = fd;
            r = r->next;
        }
    return (0);
}


char *get_path(char *cmd, char **env)
{
	int i = 0;
	char **res;
	char *path, *path_slash;

	while (*env && ft_strncmp(*env, "PATH", 4))
		env++;
	res = ft_split(*env + 5, ':');
	while (res[++i])
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
	}
	ft_free_tab(res);
	return cmd;
}

int execute(t_command *cmd, t_env **env_list, char **env, t_shell *mini)
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
	if (handle_redirection(current, (*env_list), mini) < 0)
    {
        mini->last_status = 1;
        if (current->next)
        {
            if (pipe(pipe_fd) < 0)
                perror("pipe");
            close(pipe_fd[1]);      // yazma ucunu kapat
            prev_fd = pipe_fd[0];   // okuma ucunu next komut için sakla
            current = current->next; // bir sonraki komuta geç
			mini->last_status = 0;
            continue;
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
			free_env_list((*env_list));
			free_struct(mini);
			execve(get_path(current->args[0], env), current->args, env);
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
