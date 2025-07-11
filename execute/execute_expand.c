/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:10:25 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/11 20:40:37 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char *append_str(char *base, const char *suffix)
{
	char *new;
	if (base)
		new = ft_strjoin(base, suffix);
	else
		new = ft_strjoin("", suffix);
	free(base);
	return new;
}

char *expand_env_var_str(const char *str, t_env *env_list)
{
	char *result = NULL;
	char *tmp2;
	int  i = 0;

	while (str[i])
	{
		if (str[i] == '\'')
		{
			int start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			tmp2 = ft_substr(str, start, i - start);
			result = append_str(result, tmp2);
			free(tmp2);
			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '$')
		{
			if (str[i + 1] == '\0' || str[i + 1] == ' ' || str[i + 1] == '"' || str[i + 1] == '\'')
			{
				tmp2 = ft_strdup("$");
				i++;
			}
			else
			{
				int start = ++i;
				while (ft_isalnum(str[i]) || str[i] == '_')
					i++;
				char *var_name = ft_substr(str, start, i - start);
				char *val = NULL;
				t_env *cur = env_list;
				while (cur)
				{
					if (ft_strcmp(cur->key, var_name) == 0)
					{
						val = ft_strdup(cur->value);
						break;
					}
					cur = cur->next;
				}
				free(var_name);
				tmp2 = val ? val : ft_strdup("");
			}
			result = append_str(result, tmp2);
			free(tmp2);
		}
		else
		{
			tmp2 = ft_substr(str, i, 1);
			result = append_str(result, tmp2);
			free(tmp2);
			i++;
		}
	}
	if (!result)
		result = ft_strdup("");
	return result;
}

char *expand_string(const char *input, t_env *env_list, int last_status)
{
	char *result = NULL;
	char *tmp2;
	int   i = 0;

	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
		{
			tmp2 = ft_itoa(last_status);
			result = append_str(result, tmp2);
			free(tmp2);
			i += 2;
		}
		else if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			int start = ++i;
			while (ft_isalnum(input[i]) || input[i] == '_')
				i++;
			char *var_name = ft_substr(input, start, i - start);
			char *val = NULL;
			t_env *cur = env_list;
			while (cur)
			{
				if (ft_strcmp(cur->key, var_name) == 0)
				{
					val = ft_strdup(cur->value);
					break;
				}
				cur = cur->next;
			}
			free(var_name);
			tmp2 = val ? val : ft_strdup("");
			result = append_str(result, tmp2);
			free(tmp2);
		}
		else
		{
			tmp2 = ft_substr(input, i, 1);
			result = append_str(result, tmp2);
			free(tmp2);
			i++;
		}
	}
	if (!result)
		result = ft_strdup("");
	return result;
}
