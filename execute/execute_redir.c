/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:04:24 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 23:56:21 by ktoraman         ###   ########.fr       */
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

int	handle_heredoc(t_redir *r, t_carry *carry, t_command *cmd,
		t_command *free_cmd)
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
		heredoc_child(carry, pipefd, cmd, free_cmd);
	return (set_heredoc_parent(pid, pipefd, &status, r));
}

t_carry	*init_carry(t_env *env_list, t_shell *mini)
{
	t_carry	*carry;

	if (!env_list || !mini)
		return (NULL);
	carry = malloc(sizeof(t_carry));
	if (!carry)
		return (NULL);
	carry->env_list = env_list;
	carry->mini = mini;
	return (carry);
}

int	handle_redirection(t_command *cmd, t_env *env_list, t_shell *mini,
		t_command *free_cmd)
{
	int		fd;
	t_redir	*r;
	t_carry	*carry;

	carry = init_carry(env_list, mini);
	r = cmd->redirs;
	while (r)
	{
		if (r->flag == R_HEREDOC)
			fd = handle_heredoc(r, carry, cmd, free_cmd);
		else
			fd = open(r->target, r->flag, 0644);
		if (fd < 0)
		{
			check_permissions(r->target);
			return (free(carry), -1);
		}
		r->fd = fd;
		if (r->flag == O_RDONLY || r->flag == R_HEREDOC)
			cmd->input = fd;
		else
			cmd->output = fd;
		r = r->next;
	}
	return (free(carry), 0);
}
