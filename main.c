/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:55:53 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 18:43:48 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

static void	ft_init_shell(t_shell *minishell)
{
	ft_memset(minishell, 0, sizeof(t_shell));
}

volatile sig_atomic_t	g_sigint_received = 0;

static int	handle_interactive(t_shell *minishell, t_env *env_list)
{
	t_command	*commands;

	while (1)
	{
		minishell->line = readline("minishell> ");
		if (!minishell->line)
			minishell->line = ft_strdup("exit");
		else if (ft_strncmp(minishell->line, "", 1) != 0)
			add_history(minishell->line);
		if (minishell->line && missing_quotes_double(minishell) == 0)
		{
			ft_token(minishell);
			ft_expand(env_list, minishell);
			minishell->args = ft_split(minishell->line, ' ');
			commands = pars(minishell->token, env_list, minishell);
			if (commands && (commands->args || commands->redirs))
				execute(commands, &env_list, minishell);
			free_less(minishell, commands);
		}
	}
	return (0);
}

static void	init_minishell(t_shell **minishell, t_env **env_list, char **env)
{
	*minishell = malloc(sizeof(t_shell));
	setup_signals();
	ft_init_shell(*minishell);
	*env_list = NULL;
	init_env(env, env_list);
}

int	main(int ac, char **av, char **env)
{
	t_shell	*minishell;
	t_env	*env_list;

	init_minishell(&minishell, &env_list, env);
	if (ac == 1 && av[0])
	{
		handle_interactive(minishell, env_list);
		free_struct(minishell);
	}
	else
	{
		error_message(minishell, "Invalid arguments", env_list);
		free_struct(minishell);
		free_env(env_list);
		return (1);
	}
	return (0);
}
