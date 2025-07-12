/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:32:48 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/12 21:34:08 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"

static int	handle_pipe_token(t_token **token, t_command **current,
		t_shell *mini, t_command *head)
{
	(*current)->next = init_command();
	if (!(*current)->next)
		return (0);
	*current = (*current)->next;
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

static int	handle_pipe_syntax(t_token *token, t_command *head)
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

static int	is_token_argument(t_token *token)
{
	return (token->type == T_WORD || token->type == T_ENV_VAR);
}

static int	process_token(t_token **token, t_command **current, t_shell *mini,
		t_command *head)
{
	if ((*token)->type == T_PIPE)
		return (handle_pipe_token(token, current, mini, head));
	else if (!is_token_argument(*token))
		return (handle_redirection_parse(token, *current, mini));
	add_arg_to_command(*current, (*token)->value);
	*token = (*token)->next;
	return (1);
}

t_command	*pars(t_token *token, t_env *env, t_shell *mini)
{
	t_command	*head;
	t_command	*current;

	(void)env;
	head = init_command();
	if (!head || !handle_pipe_syntax(token, head))
		return (NULL);
	current = head;
	while (token)
	{
		if (!process_token(&token, &current, mini, head))
			return (free_commands(head), NULL);
	}
	return (head);
}
