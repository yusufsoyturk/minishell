/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:34 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 16:53:39 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_remover_token(t_shell *mini)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*next;

	curr = mini->token;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->type && curr->value && curr->value[0] == '\0')
		{
			if (prev)
				prev->next = curr->next;
			else
				mini->token = curr->next;
			free(curr->value);
			free(curr);
		}
		else
			prev = curr;
		curr = next;
	}
}

void	handle_heredoc_expand(t_token *token)
{
	token->here_flag = 0;
	if (token->next && quotes_controler(token->next) == 1)
		token->here_flag = 1;
}

static void	expand_token_value(t_env *env_list, t_shell *mini)
{
	int		i;
	char	*new_value;

	i = 0;
	new_value = NULL;
	if (dollar_control(mini->token) == 1)
	{
		new_value = expand_with_quotes(env_list, mini, &i);
		if (mini->token->value)
		{
			free(mini->token->value);
			mini->token->value = new_value;
		}
	}
}

static void	iterate_tokens(t_env *env_list, t_shell *mini)
{
	t_token	*head;

	head = mini->token;
	while (mini->token)
	{
		if (mini->token->type == T_REDIR_HEREDOC)
		{
			handle_heredoc_expand(mini->token);
			if (mini->token->next && mini->token->next->next)
				mini->token = mini->token->next->next;
			else
				break ;
		}
		if (mini->token->type == T_WORD || mini->token->type == T_ENV_VAR)
			expand_token_value(env_list, mini);
		mini->token = mini->token->next;
	}
	mini->token = head;
}

void	ft_expand(t_env *env_list, t_shell *mini)
{
	iterate_tokens(env_list, mini);
	ft_remover_token(mini);
	remove_quotes(mini->token);
}
