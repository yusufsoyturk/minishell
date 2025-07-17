/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:59:33 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/17 14:04:56 by ysoyturk         ###   ########.fr       */
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

static char	*handle_normal_char(t_token *token, t_shell *mini, int *i)
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

char	*expand_env_var(t_env *env_list, t_token *token, int *i, t_shell *mini)
{
	char	*new_val;
	char	*tmp;
	char	*val;

	new_val = NULL;
	while (*i < (int)ft_strlen(token->value) && token->value[*i])
	{
		if (ft_strncmp(token->value + *i, "$?", 2) == 0)
			val = handle_dollar_question_quo(mini, &val , i);
		else if (token->value[*i] == '$')
			val = handle_dollar(env_list, token, i);
		else if (token->value[*i] == 39 && mini->d_flag == 0)
			val = handle_single_quote(env_list, token, i);
		else
			val = handle_normal_char(token, mini, i);
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
