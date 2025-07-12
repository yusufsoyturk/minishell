/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:41:28 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 18:48:48 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"

int	handle_pipe_syntax(t_token *token, t_command *head)
{
	if (!token)
	{
		free_commands(head);
		return (0);
	}
	if (token->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		free_commands(head);
		return (0);
	}
	return (1);
}

int	handle_pipe_token(t_token **token, t_command **cur, t_shell *mini,
		t_command *head)
{
	(*cur)->next = init_command();
	*cur = (*cur)->next;
	*token = (*token)->next;
	if (!(*token))
	{
		mini->last_status = 2;
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		free_commands(head);
		return (0);
	}
	if (token_check_pipe((*token)->value, mini))
	{
		free_commands(head);
		return (0);
	}
	return (1);
}
