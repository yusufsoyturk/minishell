/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 00:50:27 by ktoraman         ###   ########.fr       */
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
		close(ctx->prev_fd);
	if (ctx->current->next)
	{
		close(ctx->pipe_fd[1]);
		ctx->prev_fd = ctx->pipe_fd[0];
	}
	ctx->loop = 1;
}

static void	wait_and_status(t_exec_ctx *ctx)
{
	int	sig;

	while (waitpid(-1, &ctx->status, 0) > 0)
		;
	if (WIFSIGNALED(ctx->status))
	{
		sig = WTERMSIG(ctx->status);
		if (sig == SIGINT)
			write(1, "\n", 1);
	}
	setup_signals();
	if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGPIPE)
		ctx->mini->last_status = 0;
	else if (WIFEXITED(ctx->status))
		ctx->mini->last_status = WEXITSTATUS(ctx->status);
	else if (WIFSIGNALED(ctx->status))
		ctx->mini->last_status = 128 + WTERMSIG(ctx->status);
	else
		ctx->mini->last_status = 1;
}

static void	init_exec_ctx(t_exec_ctx *ctx, t_command *cmd, t_env **env_list,
		t_shell *mini)
{
	ctx->current = cmd;
	ctx->env_list = env_list;
	ctx->mini = mini;
	ctx->cmd = cmd;
	ctx->prev_fd = -1;
	ctx->status = 0;
	ctx->loop = 0;
}

int	execute(t_command *cmd, t_env **env_list, t_shell *mini)
{
	t_exec_ctx	ctx;
	pid_t		pid;
	int			ret;

	init_exec_ctx(&ctx, cmd, env_list, mini);
	while (ctx.current)
	{
		mini->last_status = 0;
		ret = check_builtin_and_pipe(&ctx);
		if (ret != -2)
			return (ret);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		ignore_signals();
		if (pid == 0)
			child_process(&ctx);
		else
			parent_process(&ctx);
		ctx.current = ctx.current->next;
	}
	wait_and_status(&ctx);
	return (mini->last_status);
}
