// #include "../inc/minishell.h"

// int	expand_env_key_len(char *env_list)
// {
// 	int i;
// 	int	env_len;
	
// 	i = 0;
// 	env_len = 0;
// 	while (env_list[i])
// 	{
// 		if (env_list[i] != 32 && env_list[i] != 34 && env_list[i] != 39)
// 			env_len++;
// 		i++;
// 	}
// 	return (env_len);
// }

// char	*expand_env_value(t_env *env_list, t_token *token)
// {
// 	t_env	*head;
// 	int	env_len;
// 	char *env_value;
// 	int	i;

// 	i = 0;
// 	env_len = 0;
// 	head = env_list;
// 	if (token->value[i] == 34)
// 	{
// 		while (token->value[i] && token->value[i] != '$')
// 			i++;
// 	}
// 	while (env_list)
// 	{
// 		env_len = expand_env_key_len(token->value[i + 1]);
// 		if (ft_strncmp(token->value + i + 1, env_list->key, env_len))
// 		{
// 			env_value = env_list->value;
// 		}
// 		env_list = env_list->next;
// 	}
// 	return (env_list);
// }

// char *expand_pre_env(); 

// void	expand_with_quotes(t_env *env_list, t_token *token)
// {
// 	int	i;
// 	int pre_len;
// 	int env_len;
// 	char *env_value;

// 	pre_len = 0;
// 	i = 0;
// 	env_len = 0;
// 	head = env_list;
// 	if (token->value[i] == 34)
// 	{
// 		while (token->value[i] && token->value[i] != '$')
// 		{
// 			pre_len++;
// 			i++;
// 		}
// 		if (token->value[i] == '$')
// 		{

// 		}
// 	}
// }

// void	expand_env_var(t_env *env_list, t_token *token)
// {
// 	t_env *head;

// 	head = env_list;
// 	while (env_list)
// 	{
// 		if (ft_strcmp(token->value + 1, env_list->key) == 0)
// 			token->value = env_list->value;
// 		env_list = env_list->next;
// 	}
// 	env_list = head;
// }

// void	ft_expand(t_env *env_list, t_token *token)
// {
// 	int str_len;

// 	(void)str_len;
// 	str_len = 0;
// 	while (token)
// 	{
// 		if (token->type == T_ENV_VAR)
// 		{
// 			expand_env_var(env_list, token);
// 		}
// 		else if (token->type == T_WORD)
// 		{
// 			printf("%s", "yeteeeeeeeeeeeeeeeeetrrrrrrrrrr");
// 		}
// 		token = token->next;
// 	}
// 	printf("%d\n", str_len);
// }
