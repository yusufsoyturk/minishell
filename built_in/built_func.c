#include "../inc/minishell.h"

int builtin_env(t_env *env_list)
{
	t_env *current;

	current = env_list;
	if (!env_list)
		return (1);
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
	return (0);
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

int	builtin_unset(const char *key, t_env **env_list)
{
	t_env	*current;
	t_env	*previous;

	current = *env_list;
	previous = NULL;
	if (!current->key || key)
		return (1);
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
			return (0);
		}
		previous = current;
		current = current->next;
	}
	return (1);
}

int	builtin_echo(char **args)
{
	int	i = 1;
	int	newline = 1;

	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
