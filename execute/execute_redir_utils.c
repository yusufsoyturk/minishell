/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:38:57 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/17 11:46:47 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_heredoc_sig(int pipefd[2])
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	close(pipefd[0]);
}

static void	heredoc_write_line(const char *expanded, int fd)
{
	ft_putendl_fd((char *)expanded, fd);
	free((char *)expanded);
}

static char	*heredoc_expand(t_redir *r, char *raw, t_env *env, t_shell *mini)
{
	if (r->here_flag == 0)
		return (expand_string(raw, env, mini->last_status));
	return (ft_strdup(raw));
}

static void	heredoc_child_exit(t_command *cmd, t_carry *carry, int pipefd[2])
{
	free_max(carry->mini, carry->env_list, cmd);
	free(carry);
	close(pipefd[1]);
	exit(130);
}

void	heredoc_child(t_carry *carry, int pfd[2], t_redir *redir,
		t_command *free_cmd)
{
	char	*raw;
	char	*expanded;

	setup_heredoc_signals();
	close(pfd[0]);
	while (1)
	{
		raw = readline("> ");
		if (g_sigint_received)
			heredoc_child_exit(free_cmd, carry, pfd);
		if (!raw)
			break ;
		if (ft_strcmp(raw, redir->target) == 0)
		{
			free(raw);
			break ;
		}
		expanded = heredoc_expand(redir, raw, carry->env_list, carry->mini);
		free(raw);
		heredoc_write_line(expanded, pfd[1]);
	}
	free_max(carry->mini, carry->env_list, free_cmd);
	free(carry);
	close(pfd[1]);
	exit(0);
}
