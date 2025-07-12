/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:17:26 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 17:17:48 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*make_token(char *line, t_token *head)
{
	int		j;
	int		*i;
	char	*token;

	j = 0;
	i = &j;
	while (line[*i])
	{
		if ((line[*i] == 32 || (line[*i] >= 9 && line[*i] <= 13)))
			(*i)++;
		else
		{
			token = find_value(line, i);
			head = new_token(head, token);
			free(token);
		}
	}
	return (head);
}

int	ft_token(t_shell *minishell)
{
	char	*line;

	minishell->token = NULL;
	line = minishell->line;
	minishell->token = make_token(line, minishell->token);
	return (0);
}
