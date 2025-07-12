/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:21:14 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 18:53:03 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"

static int	is_token_argument(t_token *token)
{
	return (token->type == T_WORD || token->type == T_ENV_VAR);
}

static int	handle_token(t_token **token, t_command **cur,
		t_shell *mini, t_command *head)
{
	if ((*token)->type == T_PIPE)
		return (handle_pipe_token(token, cur, mini, head));
	else if (!is_token_argument(*token))
		return (handle_redirection_parse(token, *cur, mini));
	add_arg_to_command(*cur, (*token)->value);
	*token = (*token)->next;
	return (1);
}

t_command	*pars(t_token *token, t_env *env, t_shell *mini)
{
	t_command	*head;
	t_command	*cur;

	(void)env;
	head = init_command();
	if (!head || !handle_pipe_syntax(token, head))
		return (NULL);
	cur = head;
	while (token)
	{
		if (!handle_token(&token, &cur, mini, head))
			return (free_commands(head), NULL);
	}
	return (head);
}
