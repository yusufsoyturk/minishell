#include "../inc/minishell.h"

int handle_heredoc(t_redir *redir)
{
	int pipefd[2];
	char *line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (g_sigint == 1)
		{
			free(line);
			close(pipefd[0]);
			close(pipefd[1]);
			return (-1);
		}
		if (!line)	
			break;
		if (ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	redir->fd = pipefd[0];
	return (pipefd[0]);
}

int handle_redirection(t_command *cmd)
{
	int			fd;
	t_command	*cur;
	t_redir		*r;

	r = cmd->redirs;
	cur = cmd;
	while (cur)
	{
		while(r)
		{
			if (r->flag == R_HEREDOC)
				fd = handle_heredoc(r);
			else
				fd = open(r->target, r->flag, 0777);
			if (fd < 0)
				return (perror(r->target), -1);
			r->fd = fd;
			if (r->flag == O_RDONLY || r->flag == R_HEREDOC)
				cur->input = fd;
			else
				cur->output = fd;
			r = r->next;
		}
		cur = cur->next;
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
	t_command *current;
	pid_t pid;
	int pipe_fd[2];
	int prev_fd;
	int status;

	prev_fd = -1;
	status = 0;
	current = cmd;
	if (handle_redirection(cmd) < 0)
		exit(1);
	current = cmd;
	while (current)
	{
		if (!current->next && is_builtin(current->args))
			return (built(current, env_list, mini));
		if (current->next && pipe(pipe_fd) < 0)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		signal(SIGINT, SIG_IGN);
		if (pid == 0)
		{
			struct sigaction sa;
    		sa.sa_handler = SIG_DFL;
    		sigemptyset(&sa.sa_mask);
    		sa.sa_flags = 0;
    		sigaction(SIGINT, &sa, NULL);
    		sigaction(SIGQUIT, &sa, NULL);
			if (current->input != STDIN_FILENO)
				dup2(current->input, STDIN_FILENO);
			else if (prev_fd != -1)
				dup2(prev_fd, STDIN_FILENO);
			if (current->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (is_builtin(current->args))
				exit(built(current, env_list, mini));
			execve(get_path(current->args[0], env), current->args, env);
			ft_putstr_fd(current->args[0], 2);
			ft_putendl_fd(": Command not found", 2);
			exit(127);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				if (sig == SIGINT)
					write(1, "\n", 1);
			}
			if (prev_fd != -1)
				close(prev_fd);
			if (current->next)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			setup_signals();
		}
		current = current->next;
	}
	return (WEXITSTATUS(status));
}
