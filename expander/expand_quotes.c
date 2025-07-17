/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 16:52:07 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/17 16:07:31 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*expand_with_quotes(t_env *env_list, t_shell *mini, int *i)
{
	char	*new_val;

	mini->d_flag = 0;
	new_val = NULL;
	while (*i < (int)ft_strlen(mini->token->value) && mini->token->value[*i])
	{
		if (is_dollar_question(mini->token, *i))
			handle_dollar_question(mini, &new_val, i);
		else if (is_double_dollar(mini->token, *i))
			handle_double_dollar(&new_val, i);
		else if (is_special_dollar(mini->token, *i))
			handle_special_dollar(&new_val, i);
		else if (mini->token->value[*i] == '$')
			handle_env_var(env_list, mini, &new_val, i);
		else if (mini->token->value[*i] == 39 && mini->d_flag == 0)
		{
			handle_s_quote(env_list, mini, &new_val, i);
		}
		else
		{
			handle_normal(env_list, mini, &new_val, i);
		}
	}
	return (new_val);
}

int	dollar_control(t_token *token)
{
	int	i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	quotes_controler(t_token *token)
{
	int	i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == 34 || token->value[i] == 39)
			return (1);
		i++;
	}
	return (0);
}

char	*handle_dollar_question_quo(t_shell *mini, char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = ft_itoa(mini->last_status);
	*new_val = ft_strdup(tmp2);
	free(tmp2);
	*i += 2;
	return (*new_val);
}
