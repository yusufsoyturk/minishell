/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:34 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/11 22:44:11 by ysoyturk         ###   ########.fr       */
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
			new_val = ft_strdup(env_list->value);
			break;
		}
		env_list = env_list->next;
	}
	free(key);
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
	char *c;

    new_val = NULL;
    while (*i < (int)ft_strlen(token->value) && token->value[*i])
    {
        if (token->value[*i] == '$')
        {
			if (token->value[*i + 1] == 34 || token->value[*i + 1] == 32)
			{
				val = ft_strdup("$");
				(*i)++;
			}
			else
			{
	            val = expand_chance_env(env_list, token, i);
			}
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
		else if (token->value[*i] == 39)
		{
			// Tek tırnak durumu - c değişkenini kullanarak
			c = expand_pre_quo(env_list, token, i);
			if (!new_val)
				new_val = c;
			else
			{
				tmp = ft_strjoin(new_val, c);
				free(new_val);
				new_val = tmp;
				free(c);  // ← Bu satır eksikti, leak'in sebebi buydu
			}
		}
        else
        {
            start = *i;
            (*i)++;
			c = ft_substr(token->value, start, 1);
            if (!new_val)
                new_val = c;
            else
            {
                tmp = ft_strjoin(new_val, c);
                free(new_val);
                new_val = tmp;
				free(c);
            }
        }
    }
	if (!new_val)
		new_val = ft_strdup("");
    return (new_val);
}

char *expand_pre(t_env *env_list, t_token *token, int *i, int *d_flag)
{
	int	pre;
	int pre2;
	int a;
	char *new_val;

	(void)*env_list;
	a = 0;
	new_val = NULL;
	pre = *i;
	pre2 = *i;
	while (token->value[pre] && token->value[pre] != '$')
	{
		if (token->value[pre] == 34 && *d_flag == 0)
		{
			*d_flag = 1;
		}
		else if (token->value[pre] == 34 && *d_flag == 1)
			*d_flag = 0;
		pre++;
	}
	new_val = malloc(sizeof(char) * pre + 1);
	while (pre > pre2)
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
	end++;
	while (token->value[end] && token->value[end] != 39)
		end++;
	end++;
	new_val = malloc(sizeof(char) * (end - (*i) + 1));
	while ((*i) < end)
	{
		new_val[a] = token->value[*i];
		a++;
		(*i)++;
	}
	new_val[a] = '\0';
	return (new_val);
}

char *expand_with_quotes(t_env *env_list, t_shell *mini, int *i)
{
    char *new_val;
    char *tmp;
    char *tmp2;
	int	d_flag;

	d_flag = 0;
    new_val = NULL;
    while (mini->token->value[*i])
    {
        // $? durumu - exit status
        if (ft_strncmp(mini->token->value + *i, "$?", 2) == 0)
        {
            tmp2 = ft_itoa(mini->last_status);
            if (!new_val)
                new_val = tmp2;
            else
            {
                tmp = ft_strjoin(new_val, tmp2);
                free(new_val);
                new_val = tmp;
                free(tmp2);
            }
            *i += 2;
        }
        // $ durumu - literal $ yazdır (sonraki karakter $ ise)
        else if (ft_strncmp(mini->token->value + *i, "$", 2) == 0)
        {
            tmp2 = ft_strdup("$");
            if (!new_val)
                new_val = tmp2;
            else
            {
                tmp = ft_strjoin(new_val, tmp2);
                free(new_val);
                new_val = tmp;
                free(tmp2);
            }
            *i += 2;
        }
        // Tek başına $ - sonraki karakter özel değilse literal $ yazdır
        else if (mini->token->value[*i] == '$' && 
                 (mini->token->value[*i + 1] == '\0' || 
                  mini->token->value[*i + 1] == ' ' || 
                  mini->token->value[*i + 1] == '"' || 
                  mini->token->value[*i + 1] == '$'))
        {
            tmp2 = ft_strdup("$");
            if (!new_val)
                new_val = tmp2;
            else
            {
                tmp = ft_strjoin(new_val, tmp2);
                free(new_val);
                new_val = tmp;
                free(tmp2);
            }
            (*i)++;
        }
        // Environment variable expansion
        else if (mini->token->value[*i] == '$')
        {
            tmp2 = expand_env_var(env_list, mini->token, i);
            if (!new_val)
                new_val = tmp2;
            else
            {
                tmp = ft_strjoin(new_val, tmp2);
                free(new_val);
                new_val = tmp;
                free(tmp2);
            }
        }
        // Single quote durumu
        else if (mini->token->value[*i] == 39 && d_flag == 0)  // 39 = single quote
        {
            tmp2 = expand_pre_quo(env_list, mini->token, i);
            if (!new_val)
                new_val = tmp2;
            else
            {
                tmp = ft_strjoin(new_val, tmp2);
                free(new_val);
                new_val = tmp;
                free(tmp2);
            }
        }
        // Normal karakterler
        else
        {
            tmp2 = expand_pre(env_list, mini->token, i, &d_flag);
            if (!new_val)
                new_val = tmp2;
            else
            {
                tmp = ft_strjoin(new_val, tmp2);
                free(new_val);
                new_val = tmp;
                free(tmp2);
            }
        }
    }
    
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

int	quotes_controler(t_token *token)
{
	int i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == 34 || token->value[i] == 39)	
			return (1);
		i++;
	}
	return (0);
}

void	ft_remover_token(t_shell *mini)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*next;

	curr = mini->token;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->type && curr->value && curr->value[0] == '\0')
		{
			if (prev)
				prev->next = curr->next;
			else
				mini->token = curr->next;

			free(curr->value);
			free(curr);
		}
		else
			prev = curr;
		curr = next;
	}
}


void	ft_expand(t_env *env_list, t_shell *mini)
{
	int	i;
	char *new_value;
	t_token *head;

	i = 0;
	new_value = NULL;
	head = mini->token;
	while (mini->token)
	{
		i = 0;
		if (mini->token->type == T_REDIR_HEREDOC)
		{
			mini->token->here_flag = 0;
			if (mini->token->next && quotes_controler(mini->token->next) == 1)
				mini->token->here_flag = 1;
			if (mini->token->next && mini->token->next->next)
				mini->token = mini->token->next->next;
			else
				break ;
		}
		if (mini->token->type == T_WORD || mini->token->type == T_ENV_VAR)
		{
			if (dollar_control(mini->token) == 1)
			{
				new_value = expand_with_quotes(env_list, mini, &i);
				if (mini->token->value)
				{
					free(mini->token->value);
					mini->token->value = new_value;
				}
			}
		}
		mini->token = mini->token->next;
	}
	mini->token = head;
	ft_remover_token(mini);
	remove_quotes(mini->token);
}
