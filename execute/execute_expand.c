/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:10:25 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 00:11:05 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*append_str(char *base, const char *suffix)
{
	char	*new;

	if (base)
		new = ft_strjoin(base, suffix);
	else
		new = ft_strjoin("", suffix);
	free(base);
	return (new);
}

static char	*lookup_env_value(const char *var_name, t_env *env_list)
{
	t_env	*cur;

	cur = env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, var_name) == 0)
			return (ft_strdup(cur->value));
		cur = cur->next;
	}
	return (ft_strdup(""));
}

static char	*handle_envvar(const char *input, int *i, t_env *env_list)
{
	int		start;
	char	*var_name;
	char	*val;

	start = ++(*i);
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	val = lookup_env_value(var_name, env_list);
	free(var_name);
	return (val);
}

static char	*handle_literal(const char *input, int *i)
{
	char	*tmp;

	tmp = ft_substr(input, *i, 1);
	(*i)++;
	return (tmp);
}

char	*expand_string(const char *put, t_env *env_list, int last_status)
{
	char	*result;
	char	*part;
	int		i;
	char	*code;

	result = NULL;
	i = 0;
	while (put[i])
	{
		if (put[i] == '$' && put[i + 1] == '?')
		{
			code = ft_itoa(last_status);
			i += 2;
			part = code;
		}
		else if (put[i] == '$' && (ft_isalpha(put[i + 1]) || put[i + 1] == '_'))
			part = handle_envvar(put, &i, env_list);
		else
			part = handle_literal(put, &i);
		result = append_str(result, part);
		free(part);
	}
	if (result == NULL)
		result = ft_strdup("");
	return (result);
}
