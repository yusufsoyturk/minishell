/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:59:33 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/17 14:38:59 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*expand_get_val(t_env *env_list, t_token *token, int *i,
		t_shell *mini)
{
	char	*val;

	if (ft_strncmp(token->value + *i, "$?", 2) == 0)
		val = handle_dollar_question_quo(mini, &val, i);
	else if (token->value[*i] == '$')
		val = handle_dollar(env_list, token, i);
	else if (token->value[*i] == 39 && mini->d_flag == 0)
		val = handle_single_quote(env_list, token, i);
	else
		val = handle_normal_char(token, mini, i);
	return (val);
}

static void	expand_append_val(char **new_val, char *val)
{
	char	*tmp;

	if (!*new_val)
		*new_val = ft_strdup(val);
	else
	{
		tmp = ft_strjoin(*new_val, val);
		free(*new_val);
		*new_val = tmp;
	}
}

char	*expand_env_var(t_env *env_list, t_token *token, int *i, t_shell *mini)
{
	char	*new_val;
	char	*val;

	new_val = NULL;
	while (*i < (int)ft_strlen(token->value) && token->value[*i])
	{
		val = expand_get_val(env_list, token, i, mini);
		expand_append_val(&new_val, val);
		free(val);
	}
	new_val = empty_handle(new_val);
	return (new_val);
}
