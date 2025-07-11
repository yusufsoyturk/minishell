/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:04:24 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 00:21:12 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	set_heredoc_parent(pid_t pid, int pipefd[2], int *status, t_redir *r)
{
	close(pipefd[1]);
	waitpid(pid, status, 0);
	setup_signals();
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
	{
		handle_heredoc_sig(pipefd);
		return (-1);
	}
	r->fd = pipefd[0];
	return (pipefd[0]);
}

int	handle_heredoc(t_redir *r, t_env *env, t_shell *mini, t_command *cmd, t_command *free_cmd)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	ignore_signals();
	if (pid == 0)
		heredoc_child(mini, env, pipefd, cmd, free_cmd);
	return (set_heredoc_parent(pid, pipefd, &status, r));
}

int	handle_redirection(t_command *cmd, t_env *env_list, t_shell *mini, t_command *free_cmd)
{
	int		fd;
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->flag == R_HEREDOC)
			fd = handle_heredoc(r, env_list, mini, cmd, free_cmd);
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
