#include "../inc/minishell.h"

t_env *find_env_node(t_env *env, const char *key)
{
    size_t	key_len;

	key_len = ft_strlen(key);
    while (env)
    {
        if (ft_strncmp(env->key, key, key_len) == 0 && env->key[key_len] == '\0')
            return (env);
        env = env->next;
    }
    return (NULL);
}


void append_env(t_env **env_list, t_env *new)
{
    t_env *current;

    if (!*env_list)
    {
        *env_list = new;
        return;
    }
    current = *env_list;
    while (current->next)
        current = current->next;
    current->next = new;
}

int	env_size(t_env *env)
{
	int i;

	i = 0;
	while (env)
	{
		if (env->exported)
			i++;
		env = env->next;
	}
	return (i);
}

t_env	**env_to_array(t_env *env)
{
	int		i = 0;
	int		size;
	t_env	**arr;

	size = env_size(env);
	arr = (t_env **)malloc(sizeof(t_env *) * (size + 1));
	if (!arr)
		return (NULL);
	while (env)
	{
		if (env->exported)
			arr[i++] = env;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	sort_env_array(t_env **arr)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i]->key, arr[j]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
