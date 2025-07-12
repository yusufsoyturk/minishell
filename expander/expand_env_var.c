/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:59:33 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 17:01:08 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*handle_dollar(t_env *env_list, t_token *token, int *i)
{
	char	*val;

	if (token->value[*i + 1] == 34 || token->value[*i + 1] == 32)
	{
		val = ft_strdup("$");
		(*i)++;
	}
	else
		val = expand_chance_env(env_list, token, i);
	return (val);
}

static char	*handle_single_quote(t_env *env_list, t_token *token, int *i)
{
	return (expand_pre_quo(env_list, token, i));
}

static char	*handle_normal_char(t_token *token, int *i)
{
	char	*val;
	int		start;

	start = *i;
	(*i)++;
	val = ft_substr(token->value, start, 1);
	return (val);
}

char	*empty_handle(char *str)
{
	if (!str)
		str = ft_strdup("");
	return (str);
}

char	*expand_env_var(t_env *env_list, t_token *token, int *i, t_shell *mini)
{
	char	*new_val;
	char	*tmp;
	char	*val;

	new_val = NULL;
	while (*i < (int)ft_strlen(token->value) && token->value[*i])
	{
		if (token->value[*i] == '$')
			val = handle_dollar(env_list, token, i);
		else if (token->value[*i] == 39 && mini->d_flag == 0)
			val = handle_single_quote(env_list, token, i);
		else
			val = handle_normal_char(token, i);
		if (!new_val)
			new_val = ft_strdup(val);
		else
		{
			tmp = ft_strjoin(new_val, val);
			free(new_val);
			new_val = tmp;
		}
		free(val);
	}
	new_val = empty_handle(new_val);
	return (new_val);
}
