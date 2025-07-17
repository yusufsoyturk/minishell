/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:38:24 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/17 14:39:38 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*handle_dollar(t_env *env_list, t_token *token, int *i)
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

char	*handle_single_quote(t_env *env_list, t_token *token, int *i)
{
	return (expand_pre_quo(env_list, token, i));
}

char	*handle_normal_char(t_token *token, t_shell *mini, int *i)
{
	int		len;
	int		a;
	char	*new_val;

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

char	*empty_handle(char *str)
{
	if (!str)
		str = ft_strdup("");
	return (str);
}
