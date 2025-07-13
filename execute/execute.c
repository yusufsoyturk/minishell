/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 16:13:17 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	check_builtin_and_pipe(t_exec_ctx *ctx)
{
	if (!ctx->current->redirs && !ctx->current->next
		&& is_builtin(ctx->current->args) && !ctx->loop)
		return (built(ctx->current, ctx->env_list, ctx->mini));
	if (ctx->current->next && pipe(ctx->pipe_fd) < 0)
		return (perror("pipe"), 1);
	return (-2);
}

static void	parent_process(t_exec_ctx *ctx)
{
	if (ctx->prev_fd != -1)
	{
		close(ctx->prev_fd);
		ctx->prev_fd = -1;
	}
	if (ctx->current->input != STDIN_FILENO)
		close(ctx->current->input);
	if (ctx->current->output != STDOUT_FILENO)
		close(ctx->current->output);
	if (ctx->current->next)
	{
		if (ctx->pipe_fd[1] != -1)
			close(ctx->pipe_fd[1]);
		ctx->prev_fd = ctx->pipe_fd[0];
		ctx->pipe_fd[0] = -1;
		ctx->pipe_fd[1] = -1;
	}
	else
	{
		if (ctx->pipe_fd[0] != -1)
			close(ctx->pipe_fd[0]);
		ctx->pipe_fd[0] = -1;
		ctx->pipe_fd[1] = -1;
	}
	ctx->loop = 1;
}

static void	wait_and_status(t_exec_ctx *ctx, pid_t *pids, int count)
{
	int	i;
	int	status;
	int	sig;
	int	last;

	last = 0;
	i = -1;
	while (++i < count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid");
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				write(1, "\n", 1);
			last = 128 + sig;
		}
		else if (WIFEXITED(status))
			last = WEXITSTATUS(status);
		else
			last = 1;
	}
	setup_signals();
	ctx->mini->last_status = last;
}

int	fork_and_execute(t_exec_ctx *ctx)
{
	pid_t	pid;
	int		ret;

	ctx->mini->last_status = 0;
	ret = check_builtin_and_pipe(ctx);
	if (ret != -2)
		return (ret);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	ctx->pids[ctx->pid_count++] = pid;
	ignore_signals();
	if (pid == 0)
		child_process(ctx);
	else
		parent_process(ctx);
	ctx->current = ctx->current->next;
	return (-2);
}

int	execute(t_command *cmd, t_env **env_list, t_shell *mini)
{
	t_exec_ctx	ctx;
	int			ret;

	init_exec_ctx(&ctx, cmd, env_list, mini);
	while (ctx.current)
	{
		if (handle_redirection(ctx.current, (*env_list), mini, cmd) < 0)
		{
			if (!handle_redirection_error(&ctx))
				return (mini->last_status);
			continue ;
		}
		ret = fork_and_execute(&ctx);
		if (ret != -2)
			return (ret);
	}
	wait_and_status(&ctx, ctx.pids, ctx.pid_count);
	return (mini->last_status);
}
