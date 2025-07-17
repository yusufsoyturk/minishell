/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:37:05 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/17 11:30:54 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_valid_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_exit_args(t_command *cmd, t_shell *mini)
{
	if (!is_valid_numeric(cmd->args[1]))
	{
		exit_error(cmd->args[1], "numeric argument required", "exit");
		mini->last_status = 2;
		return (1);
	}
	if (cmd->args[2])
	{
		ft_putendl_fd("cmd: exit: too many arguments", 2);
		mini->last_status = 1;
		return (1);
	}
	return (0);
}

int	builtin_exit(t_command *cmd, t_env *env_list, t_shell *mini,
		t_command *current)
{
	long long	code;

	write(1, "exit\n", 5);
	code = 0;
	if (current->args[1])
	{
		if (check_exit_args(current, mini))
			return (1);
		code = ft_atoll(current->args[1]) % 256;
	}
	free_max(mini, env_list, cmd);
	exit(code);
}
