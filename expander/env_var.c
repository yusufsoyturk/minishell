#include "../inc/minishell.h"

char *expand_chance_env(t_env *env_list, t_token *token, int *i)
{
	t_env *head;
	char *new_val;
	int a;

	head = env_list;
	new_val = NULL;
	(*i)++;
	a = (*i);
	while (token->value[*i] && token->value[*i] != '$' && token->value[*i] != 34 && token->value[*i] != 39)
		(*i)++;
	while (env_list)
	{
		if (ft_strncmp(token->value + a, env_list->key, *i - a) == 0)
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

char	*expand_env_var(t_env *env_list, t_token *token, int *i)
{
	char *new_val;
	char *new_val2;
	// char *tmp;

	new_val = NULL;
	new_val2 = NULL;
	// tmp = NULL;
	while (token->value[*i])
	{
		if (token->value[*i] == '$')
		{
			if (!new_val)
			{
				new_val = ft_strdup(expand_chance_env(env_list, token, i));
			}
			else if (new_val)
			{
				if (new_val2)
					free(new_val2);
				new_val2 = ft_strdup(expand_chance_env(env_list, token, i));
				new_val = ft_strjoin(new_val, new_val2);
			}
		}
		else
			break;
	}
	return (new_val);
	// printf("%d %s\n", i, new_val);
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
				new_val = ft_strdup(expand_pre_quo(env_list, token, i));
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
				new_val = ft_strdup(expand_env_var(env_list, token, i));
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
	
	i = 0;
	new_value = NULL;
	while (token)
	{
		// if (token->type == T_ENV_VAR)
		// {
		// 	// printf("%s\n", token->value);
		// 	expand_env_var(env_list, token);
		// }
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
}
