/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:40:12 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 21:44:38 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	update_or_add_env(char *arg, t_env **env_list)
{
	char	*key;
	char	*value;
	t_env	*node;
	t_env	*new;

	key = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
	value = ft_strdup(ft_strchr(arg, '=') + 1);
	node = find_env_node(*env_list, key);
	if (node)
	{
		free(node->value);
		node->value = value;
		node->exported = 1;
	}
	else
	{
		new = malloc(sizeof(t_env));
		new->key = ft_strdup(key);
		new->value = value;
		new->exported = 1;
		new->next = NULL;
		append_env(env_list, new);
	}
	free(key);
}

static void	print_export(t_env *env)
{
	t_env	**sorted;
	int		i;

	sorted = env_to_array(env);
	if (!sorted)
		return ;
	sort_env_array(sorted);
	i = 0;
	while (sorted[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(sorted[i]->key, 1);
		if (sorted[i]->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(sorted[i]->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
			ft_putstr_fd("\n", 1);
		i++;
	}
	free(sorted);
}

t_env	*create_env_node(char *key, char *value, int exported)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->exported = exported;
	new->next = NULL;
	return (new);
}

static int	is_valid_var_name(const char *name)
{
	int	i;

	i = 0;
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **args, t_env **env_list, t_shell *mini)
{
	int	i;

	if (!args[1])
	{
		print_export(*env_list);
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
			handle_invalid_identifier(args[i], mini);
		else
			handle_valid_identifier(args[i], env_list);
		i++;
	}
	return (0);
}
