#include "../inc/minishell.h"

int	handle_heredoc(t_redir *redir)// değişebilir
{
	int		pipefd[2];
	char	*line;

	setup_heredoc_signals();
	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);

	while (1)
	{
		line = readline("> ");
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

int	handle_redirection(t_command *cmd)
{
	int	fd;

	while (cmd->redirs)
	{
		fd = 0;
		if (cmd->redirs->flag == R_HEREDOC)
			fd = handle_heredoc(cmd->redirs);
		else
			fd = open(cmd->redirs->target, cmd->redirs->flag, 0644);

		if (fd < 0)
		{
			perror(cmd->redirs->target);
			return (-1);
		}
		cmd->redirs->fd = fd;
		if (cmd->redirs->flag == O_RDONLY || cmd->redirs->flag == R_HEREDOC)
			cmd->input = fd;
		else
			cmd->output = fd;
		cmd->redirs = cmd->redirs->next;		
	}
	return (0);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**res;
	char	*path;
	char	*path_slash;

	i = 0;
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
			return (path);
		}
		free(path);
	}
	ft_free_tab(res);
	return (cmd);
}

int	arg_check(char *cmd)
{
	if (ft_strcmp(cmd, "cat") == 0 ||
	ft_strcmp(cmd, "grep") == 0 ||
	ft_strcmp(cmd, "head") == 0 ||
	ft_strcmp(cmd, "tail") == 0 ||
	ft_strcmp(cmd, "cut") == 0 ||
	ft_strcmp(cmd, "awk") == 0 ||
	ft_strcmp(cmd, "sed") == 0 ||
	ft_strcmp(cmd, "unset") == 0 ||
	ft_strcmp(cmd, "sort") == 0)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Syntax error command requiers arguman", 2);
		return (1);
	}
	return (0);
}
int	execute(t_command *cmd, t_env **env_list, char **env, t_shell *mini)
{
	t_command	*current;
	pid_t		pid;
	int			pipe_fd[2];
	int			prev_fd = -1;
	int			status = 0;

	current = cmd;
	while (current)
	{
		if (arg_check(current->args[0]) && !current->args[1])
			break ;
		if (!current->next && is_builtin(current->args))
		{
			if (current->redirs && handle_redirection(current) == -1)
				return (1);
			if (current->input != STDIN_FILENO)
				dup2(current->input, STDIN_FILENO);
			if (current->output != STDOUT_FILENO)
				dup2(current->output, STDOUT_FILENO);
			return (built(current, env_list, mini));
		}
		if (current->next && pipe(pipe_fd) < 0)
			return (perror("pipe"), 1);

		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);

		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (current->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (current->redirs && handle_redirection(current) == -1)
				exit(1);
			if (current->input != STDIN_FILENO)
				dup2(current->input, STDIN_FILENO);
			if (current->output != STDOUT_FILENO)
				dup2(current->output, STDOUT_FILENO);
			execve(get_path(current->args[0], env), current->args, env);
			ft_putstr_fd(current->args[0], 2);
			ft_putendl_fd(": Command not found", 2);
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
			else if (pipe_fd[0] != -1)
				close(pipe_fd[0]);
		}
		current = current->next;
	}
	while (wait(&status) > 0)
		;
	return (WEXITSTATUS(status));
}
