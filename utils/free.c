/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 09:44:12 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/17 15:28:56 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->args)
		{
			i = 0;
			while (tmp->args[i])
			{
				free(tmp->args[i]);
				i++;
			}
			free(tmp->args);
		}
		free_redirections(tmp->redirs);
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void	free_max(t_shell *minishell, t_env *env, t_command *cmd)
{
	free_env(env);
	free_double(minishell->args);
	free_token(minishell->token);
	free(minishell->line);
	free(minishell);
	free_commands(cmd);
}

void	free_less(t_shell *minishell, t_command *commands)
{
	if (minishell->args)
	{
		free_double(minishell->args);
		minishell->args = NULL;
	}
	if (minishell->token)
		free_token(minishell->token);
	if (minishell->line)
		free(minishell->line);
	free_commands(commands);
}
