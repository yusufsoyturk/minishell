/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:04:24 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/11 14:19:44 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

volatile sig_atomic_t g_sigint_received = 0;

void heredoc_sigint_handler(int signo)
{
	(void)signo;
	close(STDIN_FILENO);
	g_sigint_received = 1;
}

static void handle_heredoc_sig(int pipefd[2])
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	close(pipefd[0]);
}
void setup_heredoc_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_heredoc_child(t_redir *redir, t_shell *mini, t_env *env_list, int pipefd[2], t_command *cmd)
{
	char	*raw;
	char	*expanded;

	setup_heredoc_signals();
	close(pipefd[0]);
	while (1)
	{
		raw = readline("> ");
		if (!raw || g_sigint_received)
			break ;
		if (ft_strcmp(raw, redir->target) == 0)
		{
			free(raw);
			break ;
		}
		if (redir->here_flag == 0)
			expanded = expand_string(raw, env_list, mini->last_status);
		else
			expanded = ft_strdup(raw);
		free(raw);
		ft_putendl_fd(expanded, pipefd[1]);
		free(expanded);
	}
	free_max(mini, env_list, cmd);
	close(pipefd[1]);
	exit(0);
}

int handle_heredoc(t_redir *redir, t_env *env_list, t_shell *mini, t_command *cmd)//PROBLEMLİ
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
		handle_heredoc_child(redir, mini, env_list, pipefd, cmd);
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		setup_signals();
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			handle_heredoc_sig(pipefd);
			return (-1);
		}
		redir->fd = pipefd[0];
		return (pipefd[0]);
	}
	return (-1);
}

int handle_redirection(t_command *cmd, t_env *env_list, t_shell *mini)
{
	int         fd;
	t_redir *r = cmd->redirs;

		while (r)
		{
			if (r->flag == R_HEREDOC)
				fd = handle_heredoc(r, env_list, mini, cmd);
			else
				fd = open(r->target, r->flag, 0644);
			if (fd < 0)
				{
					check_permissions(r->target);
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
