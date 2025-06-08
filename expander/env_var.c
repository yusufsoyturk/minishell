#include "../inc/minishell.h"

int	expand_env_key_len(char *env_list)
{
	int i;
	int	env_len;
	
	i = 0;
	env_len = 0;
	while (env_list[i])
	{
		if (env_list[i] != 32 && env_list[i] != 34 && env_list[i] != 39)
			env_len++;
		i++;
	}
	return (env_len);
}

char	*expand_env_value(t_env *env_list, t_token *token, int *a)
{
	t_env	*head;
	int	env_len;
	char *env_value;
	int	i;

	i = *a;
	env_len = 0;
	head = env_list;
    env_value = "\0";
	// if (token->value[i] == 34)
	// {
	// 	while (token->value[i] && token->value[i] != '$')
	// 	{
	// 		i++;
	// 	}
	// }
	while (env_list)
	{
		env_len = expand_env_key_len(token->value + i + 1);
		if (ft_strncmp(token->value + i + 1, env_list->key, env_len) == 0)
		{
			(*a) += ft_strlen(env_list->key) - 1; 
			env_value = env_list->value;
            break;
		}
		env_list = env_list->next;
	}
    env_list = head;
	return (env_value);
}

int	pre_env_len(t_token *token, int *a)
{
	int	i;
	int j;

	j = 0;
	i = *a;
	while (token->value[i] && token->value[i] != '$')
	{
		j++;
		i++;
	}
	return (j);
}

char *expand_pre_env(t_token *token, int *a)
{
	int	i;
	int j;
	char *str;

	i = pre_env_len(token, a);
	j = 0;
	str = malloc(sizeof(char *) * (i + 1));
	while (i > 0)
	{
		str[j] = token->value[j];
		i--;
		j++;
		(*a)++;
	}
	str[j] = '\0';
	return (str);
}

void	expand_with_quotes(t_env *env_list, t_token *token)
{
	char *str1;
	char *str2;
	char *str3;
	char *str4;
	int	*i;
	int j;
	int flag1;

	flag1 = 0;
	j = 0;
	i = &j;
	str1 = NULL;
	while (token->value[*i] && token->value[0] == 34)
	{
		if (token->value[*i] != '$')
		{
			str1 = expand_pre_env(token, i);
			printf("%s\n", str1);
			flag1 = 1;
		}
		else if (token->value[*i] == '$')
		{
			flag1 = 2;
			str2 = expand_env_value(env_list, token, i);
			printf("%s\n", str2);
		}
		if (str1 != NULL && str2 != NULL)
		{
			if (str3 == NULL)
			{
				str3 = ft_strjoin(str1, str2);
			}
			else if (flag1 == 1)
			{
				str4 = str3;
				str3 = ft_strjoin(str4, str1);
				printf("%s\n", str3);
			}
			else if (flag1 == 2)
			{
				str4 = str3;
				str3 = ft_strjoin(str4, str2);
				// printf("%s\n", str3);
			}
		}
		// free(token->value);
		// (*i)++;
	}
	token->value = str3;
}

void	expand_env_var(t_env *env_list, t_token *token)
{
	t_env *head;

	head = env_list;
	while (env_list)
	{
		if (ft_strcmp(token->value + 1, env_list->key) == 0)
			token->value = env_list->value;
		env_list = env_list->next;
	}
	env_list = head;
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
	while (token)
	{
		if (token->type == T_ENV_VAR)
		{
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
