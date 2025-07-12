/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 22:29:05 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// int	execute(t_command *cmd, t_env **env_list, t_shell *mini)
// {
// 	t_command	*current;
// 	pid_t		pid;
// 	int			pipe_fd[2];
// 	int			prev_fd;
// 	int			status;
// 	int			loop;
	
// 	loop = 0;
// 	prev_fd = -1;
// 	status = 0;
// 	current = cmd;
// 	while (current)
// 	{
// 		mini->last_status = 0;
// 		if (handle_redirection(current, (*env_list), mini, cmd) < 0)
// 		{
// 			mini->last_status = 1;
// 			if (current->next)
// 			{
// 				if (pipe(pipe_fd) < 0)
// 				perror("pipe");
// 				close(pipe_fd[1]);
// 				prev_fd = pipe_fd[0];
// 				current = current->next;
// 				mini->last_status = 0;
// 				continue ;
// 			}
// 			return (mini->last_status);
// 		}
// 		if (!current->redirs && !current->next && is_builtin(current->args) && !loop)
// 			return (built(current, env_list, mini));
// 		if (current->next && pipe(pipe_fd) < 0)
// 			return (perror("pipe"), 1);
// 		pid = fork();
// 		if (pid < 0)
// 			return (perror("fork"), 1);
// 		ignore_signals();
// 		if (pid == 0)
// 		{
// 			setup_child_signals();
// 			if (!current->args)
// 			{
// 				free_max(mini, *env_list, cmd);
// 				exit(0);
// 			}
// 			if (current->input != STDIN_FILENO)
// 			dup2(current->input, STDIN_FILENO);
// 			else if (prev_fd != -1)
// 			dup2(prev_fd, STDIN_FILENO);
// 			if (current->output != STDOUT_FILENO)
// 			dup2(current->output, STDOUT_FILENO);
// 			else if (current->next)
// 			dup2(pipe_fd[1], STDOUT_FILENO);
// 			if (current->next)
// 			{
// 				close(pipe_fd[0]);
// 				close(pipe_fd[1]);
// 			}
// 			if (is_builtin(current->args))
// 			{
// 				built(current, env_list, mini);
// 				free_struct(mini);
// 				free_env_list((*env_list));
// 				free_commands(cmd);
// 				exit(0);
// 			}
// 			if (!current->args[0] || current->args[0][0] == '\0')
// 			{
// 				ft_putstr_fd("minishell: command not found\n", 2);
// 				free_env_list((*env_list));
// 				free_struct(mini);
// 				free_commands(cmd);
// 				exit(127);
// 			}
// 			char **char_env;
// 			char_env = env_to_envp_array(*env_list);
// 			char *exec_path = get_path(current->args[0], char_env);
// 			if (!exec_path)
// 			{
// 				if (access(current->args[0], F_OK) == 0)
// 				exec_path = ft_strdup(current->args[0]);
// 				else	
// 				{
// 					ft_putstr_fd("minishell: ", 2);
// 					ft_putstr_fd(current->args[0], 2);
// 					ft_putendl_fd(": command not found", 2);
// 					free_env_list((*env_list));
// 					free_struct(mini);
// 					ft_free_tab(char_env);
// 					free_commands(cmd);
// 					exit(127);
// 				}
// 			}
// 			free_env_list((*env_list));
// 			free_struct(mini);
// 			check_permissions_exec(exec_path, cmd, char_env);
// 			execve(exec_path, current->args, char_env);
// 			ft_putstr_fd("minishell: ", 2);
// 			ft_putstr_fd(current->args[0], 2);
// 			ft_putendl_fd(": command not found", 2);
// 			ft_free_tab(char_env);
// 			free_commands(cmd);
// 			free(exec_path);
// 			exit(127);
// 		}
// 		else
// 		{
// 			if (prev_fd != -1)
// 			close(prev_fd);
// 			if (current->next)
// 			{
// 				close(pipe_fd[1]);
// 				prev_fd = pipe_fd[0];
// 			}
// 			loop = 1;
// 		}
// 		current = current->next;
// 	}
// 	while (waitpid(-1, &status, 0) > 0)
// 		;
// 	if (WIFSIGNALED(status))
// 	{
// 		int sig = WTERMSIG(status);
// 		if (sig == SIGINT)
// 		write(1, "\n", 1);
// 	}
// 	setup_signals();
// 	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
// 		mini->last_status = 0;
// 	else if (WIFEXITED(status))
// 		mini->last_status = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 		mini->last_status = 128 + WTERMSIG(status);
// 	else 
// 		mini->last_status = 1;
// 	return (mini->last_status);
// }

typedef struct s_exec_ctx
{
    t_command	*current;
    t_env		**env_list;
    t_shell		*mini;
    t_command	*cmd;
    int			pipe_fd[2];
    int			prev_fd;
    int			status;
    int			loop;
}	t_exec_ctx;

static void	handle_redir_error(t_exec_ctx *ctx)
{
    ctx->mini->last_status = 1;
    if (ctx->current->next)
    {
        if (pipe(ctx->pipe_fd) < 0)
            perror("pipe");
        close(ctx->pipe_fd[1]);
        ctx->prev_fd = ctx->pipe_fd[0];
        ctx->current = ctx->current->next;
        ctx->mini->last_status = 0;
    }
}

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
    while (waitpid(-1, &ctx->status, 0) > 0)
        ;
    if (WIFSIGNALED(ctx->status))
    {
        int sig = WTERMSIG(ctx->status);
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

    ctx.current = cmd;
    ctx.env_list = env_list;
    ctx.mini = mini;
    ctx.cmd = cmd;
    ctx.prev_fd = -1;
    ctx.status = 0;
    ctx.loop = 0;
    while (ctx.current)
    {
        mini->last_status = 0;
        if (handle_redirection(ctx.current, *env_list, mini, cmd) < 0)
        {
            handle_redir_error(&ctx);
            if (!ctx.current->next)
                return (mini->last_status);
            continue ;
        }
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
