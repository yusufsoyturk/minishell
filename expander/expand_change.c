/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_change.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:57:37 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/17 13:51:19 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*find_env_value(t_env *env_list, char *key, int key_len)
{
	while (env_list)
	{
		if (ft_strlen(env_list->key) == (size_t)key_len && ft_strncmp(key,
				env_list->key, key_len) == 0)
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}

char	*expand_chance_env(t_env *env_list, t_token *token, int *i)
{
	int		start;
	int		key_len;
	char	*key;
	char	*new_val;

	(*i)++;
	start = *i;
	while (token->value[*i] && token->value[*i] != '$' && token->value[*i] != 34
		&& token->value[*i] != 39 && token->value[*i] != 32)
		(*i)++;
	key_len = *i - start;
	key = ft_substr(token->value, start, key_len);
	new_val = find_env_value(env_list, key, key_len);
	free(key);
	return (new_val);
}
