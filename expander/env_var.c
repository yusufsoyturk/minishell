#include "../inc/minishell.h"

// int	expand_env_quotes_len(t_env *env_list, char *value, int len)
// {
// 	int	i;
// 	(void)len;
// 	i = 0;
// 	if (value[i] && value[i] == '$')
// 	{
// 		printf("try");
// 		// if (ft_strncmp())
// 	}
// }

int	expand_quotes_len(t_token *token)
{
	int	i;
	int str_len;
	int	env_key_len;

	str_len = 0;
	env_key_len = -1;
	i = 0;
	if (token->type == T_WORD && token->value[0] == 34)
	{
		while (token->value[i])
		{
			while (token->value[i] && token->value[i] != '$')
			{
				i++;
				str_len++;
			}
			while (token->value[i] && token->value[i] != 32 && token->value[i] != 34)
			{
				i++;
				env_key_len++;
				printf("%s %d\n", "env" ,env_key_len);
			}
			i++;
		}
	}
	return (str_len);
}

// void	expand_with_quotes(t_env *env_list, t_token *token)
// {

// }

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

void	ft_expand(t_env *env_list, t_token *token)
{
	int str_len;

	(void)str_len;
	str_len = 0;
	while (token)
	{
		if (token->type == T_ENV_VAR)
		{
			expand_env_var(env_list, token);
		}
		else if (token->type == T_WORD)
		{
			printf("%s", "yeteeeeeeeeeeeeeeeeetrrrrrrrrrr");
			// str_len =  expand_quotes_len(token);
		}
		token = token->next;
	}
	// printf("%d\n", str_len);
}
