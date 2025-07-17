/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:04:24 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/17 11:41:59 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	set_heredoc_parent(pid_t pid, int pipefd[2], int *status, t_redir *r)
{
	close(pipefd[1]);
	waitpid(pid, status, 0);
	setup_signals();
	if (WIFEXITED(*status) && WEXITSTATUS(*status) == 130)
	{
		handle_heredoc_sig(pipefd);
		return (-1);
	}
	r->fd = pipefd[0];
	return (pipefd[0]);
}

int	handle_heredoc(t_redir *r, t_carry *carry,
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
		heredoc_child(carry, pipefd, r, free_cmd);
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

int	handle_redirection_exec(t_command *cmd, t_env *env_list, t_shell *mini,
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
			fd = handle_heredoc(r, carry, free_cmd);
		else
			fd = open(r->target, r->flag, 0644);
		if (fd < 0)
			return (handle_fd_error(r, mini, carry));
		r->fd = fd;
		if (r->flag == O_RDONLY || r->flag == R_HEREDOC)
			cmd->input = fd;
		else
			cmd->output = fd;
		r = r->next;
	}
	return (free(carry), 0);
}

int	handle_redirection_error_exec(t_exec_ctx *ctx)
{
	if (ctx->mini->last_status != 130)
		ctx->mini->last_status = 1;
	if (ctx->current->next)
	{
		if (pipe(ctx->pipe_fd) < 0)
			perror("pipe");
		close(ctx->pipe_fd[1]);
		ctx->prev_fd = ctx->pipe_fd[0];
		ctx->current = ctx->current->next;
		ctx->mini->last_status = 0;
		return (1);
	}
	return (0);
}
