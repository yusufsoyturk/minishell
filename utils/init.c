#include "../inc/minishell.h"

void init_env(char **env, t_env **env_list)
{
	t_env   *new_node;
	int     i;
	char    *equal_pos;

	i = 0;
	while (env[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;  // malloc hatası, düşük öncelik: exit de edebilirsin

		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos)
		{
			new_node->key      = ft_substr(env[i], 0, equal_pos - env[i]);
			new_node->value    = ft_strdup(equal_pos + 1);
		}
		else
		{
			new_node->key      = ft_strdup(env[i]);
			new_node->value    = NULL;
		}
		new_node->exported = 1;
		new_node->next     = NULL;

		append_env(env_list, new_node);
		i++;
	}
}
