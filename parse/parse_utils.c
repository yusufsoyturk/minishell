/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:32:29 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 21:32:37 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input = STDIN_FILENO;
	cmd->output = STDOUT_FILENO;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	add_arg_to_command(t_command *cmd, const char *arg)
{
	int		count;
	int		i;
	char	**new_args;

	i = -1;
	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (-1);
	while (++i < count)
		new_args[i] = cmd->args[i];
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (0);
}

int	token_check_pipe(char *str, t_shell *mini)
{
	char	*prefix;
	char	*msg;
	char	*tmp;

	prefix = ft_strdup("minishell: syntax error near unexpected token `");
	if (ft_strncmp(str, "&", 1) == 0 || ft_strncmp(str, "|", 1) == 0
		|| ft_strncmp(str, "&&", 2) == 0 || ft_strncmp(str, "||", 2) == 0)
	{
		tmp = ft_strjoin(prefix, str);
		free(prefix);
		msg = ft_strjoin(tmp, "'");
		free(tmp);
		ft_putendl_fd(msg, 2);
		free(msg);
		mini->last_status = 2;
		return (1);
	}
	free(prefix);
	return (0);
}

int	token_check(char *str, t_shell *mini)
{
	char	*prefix;
	char	*msg;
	char	*tmp;

	prefix = ft_strdup("minishell: syntax error near unexpected token `");
	if (ft_strncmp(str, "<", 1) == 0 || ft_strncmp(str, ">", 1) == 0
		|| ft_strncmp(str, "<<", 1) == 0 || ft_strncmp(str, ">>", 1) == 0
		|| ft_strncmp(str, "&", 1) == 0 || ft_strncmp(str, "|", 1) == 0
		|| ft_strncmp(str, "&&", 2) == 0 || ft_strncmp(str, "||", 2) == 0)
	{
		tmp = ft_strjoin(prefix, str);
		free(prefix);
		msg = ft_strjoin(tmp, "'");
		free(tmp);
		ft_putendl_fd(msg, 2);
		free(msg);
		mini->last_status = 2;
		return (1);
	}
	free(prefix);
	return (0);
}
