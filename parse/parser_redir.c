/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:31:06 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 21:33:11 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	handle_redirection_error(t_redir *redir, t_shell *mini,
		t_command *head)
{
	free(redir);
	mini->last_status = 2;
	ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
	free_commands(head);
	return (0);
}

static void	set_redirection_flag(t_token *token, t_redir *redir)
{
	if (token->type == T_REDIR_IN)
		redir->flag = O_RDONLY;
	else if (token->type == T_REDIR_OUT)
		redir->flag = O_CREAT | O_WRONLY | O_TRUNC;
	else if (token->type == T_REDIR_APPEND)
		redir->flag = O_CREAT | O_WRONLY | O_APPEND;
	else if (token->type == T_REDIR_HEREDOC)
	{
		if (token->here_flag == 1)
			redir->here_flag = 1;
		redir->flag = R_HEREDOC;
	}
}

int	handle_redirection_parse(t_token **token, t_command *current, t_shell *mini)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (0);
	redir->next = NULL;
	redir->here_flag = 0;
	redir->fd = -1;
	set_redirection_flag(*token, redir);
	*token = (*token)->next;
	if (!(*token))
		return (handle_redirection_error(redir, mini, NULL));
	if (token_check((*token)->value, mini))
		return (free(redir), 0);
	redir->target = ft_strdup((*token)->value);
	if (!current->redirs)
		current->redirs = redir;
	else
	{
		tmp = current->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	*token = (*token)->next;
	return (1);
}
