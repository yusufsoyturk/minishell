/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 23:38:32 by ktoraman         ###   ########.fr       */
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

static void	setup_child_io(t_exec_ctx *ctx)
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

static void	child_builtin_exit(t_exec_ctx *ctx)
{
	built(ctx->current, ctx->env_list, ctx->mini);
	free_struct(ctx->mini);
	free_env_list(*(ctx->env_list));
	free_commands(ctx->cmd);
	exit(0);
}

static void	child_cmd_not_found(t_exec_ctx *ctx)
{
	ft_putstr_fd("minishell: command not found\n", 2);
	free_env_list(*(ctx->env_list));
	free_struct(ctx->mini);
	free_commands(ctx->cmd);
	exit(127);
}

static void	child_execve(t_exec_ctx *ctx, char **char_env, char *exec_path)
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

static void	child_process(t_exec_ctx *ctx)
{
	char	**char_env;
	char	*exec_path;

	if (handle_redirection(ctx->current, *(ctx->env_list), ctx->mini, ctx->cmd) < 0)
	{
		free_max(ctx->mini, *(ctx->env_list), ctx->cmd);
        exit(1);
	}
	setup_child_signals();
	if (!ctx->current->args)
	{
		free_max(ctx->mini, *(ctx->env_list), ctx->cmd);
		exit(0);
	}
	setup_child_io(ctx);
	if (is_builtin(ctx->current->args))
		child_builtin_exit(ctx);
	if (!ctx->current->args[0] || ctx->current->args[0][0] == '\0')
		child_cmd_not_found(ctx);
	char_env = env_to_envp_array(*(ctx->env_list));
	exec_path = get_path(ctx->current->args[0], char_env);
	if (!exec_path)
	{
		if (access(ctx->current->args[0], F_OK) == 0)
			exec_path = ft_strdup(ctx->current->args[0]);
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(ctx->current->args[0], 2);
			ft_putendl_fd(": command not found", 2);
			free_env_list(*(ctx->env_list));
			free_struct(ctx->mini);
			ft_free_tab(char_env);
			free_commands(ctx->cmd);
			exit(127);
		}
	}
	child_execve(ctx, char_env, exec_path);
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

int	execute(t_command *cmd, t_env **env_list, t_shell *mini)
{
	t_exec_ctx	ctx;
	pid_t		pid;
	int			ret;
	int			redir_exit;

	ctx.current = cmd;
	ctx.env_list = env_list;
	ctx.mini = mini;
	ctx.cmd = cmd;
	ctx.prev_fd = -1;
	ctx.status = 0;
	ctx.loop = 0;
	redir_exit = 0;
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
