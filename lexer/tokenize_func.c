/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:16:18 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/17 15:39:15 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*new_word(char *line)
{
	char	*token;
	int		len;
	int		i;

	i = 0;
	len = word_len(line);
	token = malloc(sizeof(char *) * len + 1);
	while (len > 0)
	{
		token[i] = line[i];
		i++;
		len--;
	}
	token[i] = '\0';
	return (token);
}

t_token_type	find_type(char *line)
{
	if (ft_strncmp(line, "|", 1) == 0)
		return (T_PIPE);
	else if (ft_strncmp(line, ">>", 2) == 0)
		return (T_REDIR_APPEND);
	else if (ft_strncmp(line, "<<", 2) == 0)
		return (T_REDIR_HEREDOC);
	else if (ft_strncmp(line, "<", 1) == 0)
		return (T_REDIR_IN);
	else if (ft_strncmp(line, ">", 1) == 0)
		return (T_REDIR_OUT);
	else if (ft_strncmp(line, "$", 1) == 0)
		return (T_ENV_VAR);
	else
		return (T_WORD);
}

char	*find_value(char *line, int *i)
{
	char	*token;

	if (line[*i] == 34)
	{
		token = in_double_quotes(line + (*i));
		*i += double_quotes_len(token);
	}
	else if (line[*i] == 39)
	{
		token = in_single_quote(line + *i);
		*i += single_quote_len(token);
	}
	else
	{
		token = new_word(line + *i);
		*i += word_len(token);
	}
	return (token);
}

t_token	*new_token(t_token *token, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->type = find_type(value);
	new->value = ft_strdup(value);
	new->next = NULL;
	new->here_flag = 0;
	ft_tknadd_back(&token, new);
	return (token);
}
