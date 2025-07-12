/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:42:12 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 18:49:11 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/parse.h"

static int	handle_redirection_error(t_redir *new, t_shell *mini)
{
	free(new);
	mini->last_status = 2;
	ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
	return (0);
}

static void	fill_redirection_type(t_token *token, t_redir *new)
{
	if (token->type == T_REDIR_IN)
		new->flag = O_RDONLY;
	else if (token->type == T_REDIR_OUT)
		new->flag = O_CREAT | O_WRONLY | O_TRUNC;
	else if (token->type == T_REDIR_APPEND)
		new->flag = O_CREAT | O_WRONLY | O_APPEND;
	else if (token->type == T_REDIR_HEREDOC)
	{
		if (token->here_flag == 1)
			new->here_flag = 1;
		new->flag = R_HEREDOC;
	}
}

void	add_redirection_to_command(t_command *cmd, t_redir *new)
{
	t_redir	*tmp;

	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	handle_redirection_parse(t_token **token, t_command *cur, t_shell *mini)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (0);
	new->next = NULL;
	new->here_flag = 0;
	new->fd = -1;
	fill_redirection_type(*token, new);
	*token = (*token)->next;
	if (!(*token))
		return (handle_redirection_error(new, mini));
	if (token_check((*token)->value, mini))
		return (free(new), 0);
	new->target = ft_strdup((*token)->value);
	add_redirection_to_command(cur, new);
	*token = (*token)->next;
	return (1);
}
