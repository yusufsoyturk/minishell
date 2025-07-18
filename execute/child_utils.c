/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:21:42 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/17 11:41:45 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redirection_error_exit(t_exec_ctx *ctx, char **char_env)
{
	char	*msg1;
	char	*msg2;

	msg1 = ft_strjoin("minishell: ", ctx->current->args[0]);
	msg2 = ft_strjoin(msg1, ": command not found\n");
	write(2, msg2, ft_strlen(msg2));
	free(msg1);
	free(msg2);
	free_env_list(*(ctx->env_list));
	free_struct(ctx->mini);
	ft_free_tab(char_env);
	free_commands(ctx->cmd);
	exit(127);
}

int	handle_child_redirection(t_exec_ctx *ctx)
{
	if (handle_redirection_exec(ctx->current, *(ctx->env_list), ctx->mini,
			ctx->cmd) < 0)
	{
		free_max(ctx->mini, *(ctx->env_list), ctx->cmd);
		return (1);
	}
	return (0);
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
		if (ft_strchr(ctx->current->args[0], '/')
			&& access(ctx->current->args[0], F_OK) == 0)
			exec_path = ft_strdup(ctx->current->args[0]);
		else
			handle_redirection_error_exit(ctx, char_env);
	}
	child_execve(ctx, char_env, exec_path);
}

int	handle_fd_error(t_redir *r, t_shell *mini, t_carry *carry)
{
	if (r->flag != R_HEREDOC)
		check_permissions(r->target);
	else
		mini->last_status = 130;
	free(carry);
	return (-1);
}
