/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_pre.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 16:03:23 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 17:01:19 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	get_pre_len(t_token *token, t_shell *mini, int i)
{
	int	len;

	len = 0;
	while (token->value[i + len] && token->value[i + len] != '$')
	{
		if (token->value[i + len] == 34 && mini->d_flag == 0)
			mini->d_flag = 1;
		len++;
	}
	return (len);
}

char	*expand_pre(t_env *env_list, t_token *token, int *i, t_shell *mini)
{
	int		len;
	int		a;
	char	*new_val;

	(void)env_list;
	len = get_pre_len(token, mini, *i);
	new_val = malloc(sizeof(char) * (len + 1));
	if (!new_val)
		return (NULL);
	a = 0;
	while (a < len)
	{
		new_val[a] = token->value[*i];
		(*i)++;
		a++;
	}
	new_val[a] = '\0';
	return (new_val);
}

char	*expand_pre_quo(t_env *env_list, t_token *token, int *i)
{
	int		end;
	char	*new_val;
	int		a;

	(void)env_list;
	a = 0;
	end = *i + 1;
	while (token->value[end] && token->value[end] != 39)
		end++;
	end++;
	new_val = malloc(sizeof(char) * (end - *i + 1));
	if (!new_val)
		return (NULL);
	while (*i < end)
	{
		new_val[a] = token->value[*i];
		a++;
		(*i)++;
	}
	new_val[a] = '\0';
	return (new_val);
}
