/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:01:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/11 16:14:25 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	env_count(t_env *env_list)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env_list;
	while (curr)
	{
		if (curr->key)
			count++;
		curr = curr->next;
	}
	return (count);
}

static char	*env_join(t_env *curr)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin(curr->key, "=");
	if (!tmp1)
		return (NULL);
	if (curr->value)
	{
		tmp2 = ft_strjoin(tmp1, curr->value);
		free(tmp1);
		if (!tmp2)
			return (NULL);
		return (tmp2);
	}
	return (tmp1);
}

static int	add_env_to_array(char **envp, t_env *curr, int i)
{
	char	*tmp;

	tmp = env_join(curr);
	if (tmp)
	{
		envp[i] = tmp;
		return (1);
	}
	return (-1);
}

char	**env_to_envp_array(t_env *env_list)
{
	int		count;
	char	**envp;
	t_env	*curr;
	int		i;

	count = env_count(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	curr = env_list;
	i = 0;
	while (curr)
	{
		if (curr->key && add_env_to_array(envp, curr, i) == 1)
			i++;
		else if (curr->key)
		{
			ft_free_tab(envp);
			return (NULL);
		}
		curr = curr->next;
	}
	envp[i] = NULL;
	return (envp);
}
