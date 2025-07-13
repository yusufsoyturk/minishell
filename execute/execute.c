/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 10:32:55 by ktoraman         ###   ########.fr       */
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

static void	wait_and_status(t_exec_ctx *ctx, pid_t *pids, int count)
{
	int	i;
	int	status;
	int	sig;
	int	last;

	last = 0;
	i = 0;
	while (i < count)
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
		i++;
	}
	setup_signals();
	ctx->mini->last_status = last;
}

static void	init_exec_ctx(t_exec_ctx *ctx, t_command *cmd, t_env **env_list,
		t_shell *mini)
{
	ctx->current = cmd;
	ctx->env_list = env_list;
	ctx->mini = mini;
	ctx->cmd = cmd;
	ctx->pid_count = 0;
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
		ctx.pids[ctx.pid_count++] = pid;
		ignore_signals();
		if (pid == 0)
			child_process(&ctx);
		else
			parent_process(&ctx);
		ctx.current = ctx.current->next;
	}
	wait_and_status(&ctx, ctx.pids, ctx.pid_count);
	return (mini->last_status);
}
