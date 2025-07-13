/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 09:44:55 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 16:11:55 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_env(char **env, t_env **env_list)
{
	t_env	*new_node;
	int		i;
	char	*equal_pos;

	i = -1;
	while (env[++i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos)
		{
			new_node->key = ft_substr(env[i], 0, equal_pos - env[i]);
			new_node->value = ft_strdup(equal_pos + 1);
		}
		else
		{
			new_node->key = ft_strdup(env[i]);
			new_node->value = NULL;
		}
		new_node->exported = 1;
		new_node->next = NULL;
		append_env(env_list, new_node);
	}
}

void	init_exec_ctx(t_exec_ctx *ctx, t_command *cmd, t_env **env_list,
		t_shell *mini)
{
	ctx->current = cmd;
	ctx->env_list = env_list;
	ctx->mini = mini;
	ctx->cmd = cmd;
	ctx->pipe_fd[0] = -1;
	ctx->pipe_fd[1] = -1;
	ctx->pid_count = 0;
	ctx->prev_fd = -1;
	ctx->status = 0;
	ctx->loop = 0;
}
