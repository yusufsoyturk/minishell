#include "../inc/minishell.h"

void builtin_env(t_env *env_list)
{
	t_env *current;

	current = env_list;
	while (current)
	{
		ft_putstr_fd(current->key, 1);
		ft_putstr_fd("=", 1);

		if (current->value)
			ft_putendl_fd(current->value, 1);
		else
			ft_putendl_fd("", 1);
		current = current->next;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void	builtin_unset(const char *key, t_env **env_list)
{
	t_env	*current;
	t_env	*previous;

	current = *env_list;
	previous = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}
