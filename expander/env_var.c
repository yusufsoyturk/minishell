/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:34 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/06/21 20:47:48 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char *expand_chance_env(t_env *env_list, t_token *token, int *i)
{
	t_env *head;
	char *new_val;
	int start;
	int key_len;
	char *key;

	head = env_list;
	new_val = NULL;
	(*i)++;
	start = (*i);
	while (token->value[*i] && token->value[*i] != '$' && token->value[*i] != 34 && token->value[*i] != 39 && token->value[*i] != 32)
		(*i)++;
	key_len = *i - start;
	key = ft_substr(token->value, start, key_len);
	while (env_list)
	{
		if (ft_strlen(env_list->key) == (size_t)key_len && ft_strncmp(key, env_list->key, key_len) == 0)
		{
			new_val = env_list->value;
			break;
		}
		env_list = env_list->next;
	}
	if (!new_val)
		new_val = ft_strdup("");
	env_list = head;
	return (new_val);
}

char *expand_env_var(t_env *env_list, t_token *token, int *i)
{
    char *new_val;
    char *tmp;
	char *val;
	int start;

    new_val = NULL;
    while (token->value[*i])
    {
        if (token->value[*i] == '$')
        {
            val = expand_chance_env(env_list, token, i);
            if (!new_val)
                new_val = ft_strdup(val);
            else
            {
                tmp = ft_strjoin(new_val, val);
                free(new_val);
                new_val = tmp;
            }
        }
		else if (token->value[*i] == 39)
		{
			if (!new_val)
				new_val = expand_pre_quo(env_list, token, i);
			else
			{
				tmp = ft_strjoin(new_val ,expand_pre_quo(env_list, token, i));
				free(new_val);
				new_val = tmp;
			}
		}
        else
        {
            start = *i;
            (*i)++;
            if (!new_val)
                new_val = ft_substr(token->value, start, 1);
            else
            {
                tmp = ft_strjoin(new_val, ft_substr(token->value, start, 1));
                free(new_val);
                new_val = tmp;
            }
        }
    }
	if (!new_val)
		new_val = ft_strdup("");
    return (new_val);
}

char *expand_pre(t_env *env_list, t_token *token, int *i)
{
	int	pre;
	int a;
	char *new_val;

	(void)*env_list;
	a = 0;
	new_val = NULL;
	pre = *i;
	while (token->value[pre] && token->value[pre] != '$' && token->value[pre] != 39)
		pre++;
	new_val = malloc(sizeof(char) * pre + 1);
	while (pre > 0)
	{
		new_val[a] = token->value[*i];
		(*i)++;
		a++;
		pre--;
	}
	new_val[a] = '\0';
	return (new_val);
}
char *expand_pre_quo(t_env *env_list, t_token *token, int *i)
{
	int end;
	char *new_val;
	int a;

	(void)*env_list;
	a = 0;
	new_val = NULL;
	end = (*i);
	while (token->value[end + 1] && token->value[end + 1] != 39)
		end++;
	end++;
	new_val = malloc(sizeof(char) * end - (*i) + 1);
	while ((*i) < end)
	{
		new_val[a] = token->value[*i];
		a++;
		(*i)++;
	}
	new_val[a] = '\0';
	if (token->value[end] == 39)
		(*i) = end + 1;
	else
		(*i) = end;
	return (new_val);
}

char	*expand_with_quotes(t_env *env_list, t_token *token, int *i)
{
	char *new_val;
	char *new_val2;

	new_val = NULL;
	new_val2 = NULL;
	while (token->value[*i])
	{
		if (token->value[*i] != 39 && token->value[*i] != '$')
		{
			if (!new_val)
				new_val = ft_strdup(expand_pre(env_list, token, i));
			else
			{
				if (new_val2)
					free(new_val2);
				new_val2 =  ft_strdup(expand_pre(env_list, token, i));
				new_val = ft_strjoin(new_val, new_val2);
			}
		}
		else if (token->value[*i] != '$')
		{
			if (!new_val)
			{
				new_val = ft_strdup(expand_pre_quo(env_list, token, i));

			}
			else
			{
				if (new_val2)
					free(new_val2);
				new_val2 = ft_strdup(expand_pre_quo(env_list, token, i));
				new_val = ft_strjoin(new_val, new_val2);
			}
		}
		else if (token->value[*i] == '$')
		{
			if (!new_val)
			{
				new_val = ft_strdup(expand_env_var(env_list, token, i));
			}
			else
			{
				if (new_val2)
					free(new_val2);
				new_val2 = ft_strdup(expand_env_var(env_list, token, i));
				new_val = ft_strjoin(new_val, new_val2);
			}
		}
		else
			(*i)++;
	}
	if (new_val2)
		free(new_val2);
	return (new_val);
}

int dollar_control(t_token *token)
{
	int i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	ft_expand(t_env *env_list, t_token *token)
{
	int	i;
	char *new_value;
	t_token *head;

	i = 0;
	new_value = NULL;
	head = token;
	while (token)
	{
		i = 0;
		if (token->type == T_WORD || token->type == T_ENV_VAR)
		{
			if (dollar_control(token) == 1)
			{
				new_value = expand_with_quotes(env_list, token, &i);
				if (token->value)
				{
					free(token->value);
					token->value = new_value;
				}
			}
			// 	expand_with_quotes(env_list, token);
		}
		token = token->next;
	}
	token = head;
	// remove_quotes(token);

}
