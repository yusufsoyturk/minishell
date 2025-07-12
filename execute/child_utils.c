/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:21:42 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 00:47:36 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redirection_error_exit(t_exec_ctx *ctx, char **char_env)
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

void	handle_child_redirection(t_exec_ctx *ctx)
{
	if (handle_redirection(ctx->current, *(ctx->env_list), ctx->mini,
			ctx->cmd) < 0)
	{
		free_max(ctx->mini, *(ctx->env_list), ctx->cmd);
		exit(1);
	}
}

void	handle_builtin_or_exec(t_exec_ctx *ctx)
{
	char	**char_env;
	char	*exec_path;

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
			handle_redirection_error_exit(ctx, char_env);
	}
	child_execve(ctx, char_env, exec_path);
}
