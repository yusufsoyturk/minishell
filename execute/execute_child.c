/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:20:55 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 14:58:21 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	setup_child_io(t_exec_ctx *ctx)
{
	if (ctx->current->input != STDIN_FILENO)
		dup2(ctx->current->input, STDIN_FILENO);
	else if (ctx->prev_fd != -1)
		dup2(ctx->prev_fd, STDIN_FILENO);
	if (ctx->current->output != STDOUT_FILENO)
		dup2(ctx->current->output, STDOUT_FILENO);
	else if (ctx->current->next)
		dup2(ctx->pipe_fd[1], STDOUT_FILENO);
	if (ctx->current->next)
	{
		close(ctx->pipe_fd[0]);
		close(ctx->pipe_fd[1]);
	}
}

void	child_builtin_exit(t_exec_ctx *ctx)
{
	built(ctx->current, ctx->env_list, ctx->mini);
	free_struct(ctx->mini);
	free_env_list(*(ctx->env_list));
	free_commands(ctx->cmd);
	exit(0);
}

void	child_cmd_not_found(t_exec_ctx *ctx)
{
	ft_putstr_fd("minishell: command not found\n", 2);
	free_env_list(*(ctx->env_list));
	free_struct(ctx->mini);
	free_commands(ctx->cmd);
	exit(127);
}

void	child_execve(t_exec_ctx *ctx, char **char_env, char *exec_path)
{
	free_env_list(*(ctx->env_list));
	free_struct(ctx->mini);
	check_permissions_exec(exec_path, ctx->cmd, char_env);
	execve(exec_path, ctx->current->args, char_env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(ctx->current->args[0], 2);
	ft_putendl_fd(": command not found", 2);
	ft_free_tab(char_env);
	free_commands(ctx->cmd);
	free(exec_path);
	exit(127);
}

void	child_process(t_exec_ctx *ctx)
{
	setup_child_signals();
	handle_builtin_or_exec(ctx);
}
