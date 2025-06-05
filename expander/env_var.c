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

void	expand_env_var(t_env *env_list, t_token *token)
{
	int	i;
	char *new_val;
	char *new_val2;
	// char *tmp;

	i = 0;
	new_val = NULL;
	new_val2 = NULL;
	// tmp = NULL;
	while (token->value[i])
	{
		if (token->value[i] == '$')
		{
			if (!new_val)
			{
				new_val = ft_strdup(expand_chance_env(env_list, token, &i));
			}
			else if (new_val)
			{
				if (new_val2)
					free(new_val2);
				new_val2 = ft_strdup(expand_chance_env(env_list, token, &i));
				new_val = ft_strjoin(new_val, new_val2);
			}
		}
		else
			i++;
	}
	free(token->value);
	token->value = new_val;
	// printf("%d %s\n", i, new_val);
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
	(void)*env_list;
	while (token)
	{
		if (token->type == T_ENV_VAR)
		{
			// printf("%s\n", token->value);
			expand_env_var(env_list, token);
		}
		else if (token->type == T_WORD)
		{
			printf("yusuf\n");
			// if (dollar_control(token) == 1)
			// 	expand_with_quotes(env_list, token);
		}
		token = token->next;
	}
}
