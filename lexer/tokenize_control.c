/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_control.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:47:32 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 18:48:50 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

static int	operator_len(char *line, int i)
{
	int	r_len;

	r_len = 0;
	while (is_operator(line[i]))
	{
		r_len++;
		i++;
		if (r_len == 2)
			return (r_len);
	}
	return (r_len);
}

static int	skip_quotes(const char *line, int i)
{
	if (line[i] == 34)
	{
		i++;
		while (line[i] && line[i] != 34)
			i++;
	}
	else if (line[i] == 39)
	{
		i++;
		while (line[i] && line[i] != 39)
			i++;
	}
	return (i);
}

int	word_len(char *line)
{
	int	i;
	int	len;

	i = -1;
	len = 0;
	while (line[++i])
	{
		i = skip_quotes(line, i);
		if (is_space(line[i]))
			break ;
		if (i > 0 && is_operator(line[i]))
		{
			if (line[i] != line[i - 1])
				break ;
			if (operator_len(line, i) == 2)
				return (2);
		}
		if (is_operator(line[i]) && line[i + 1] && line[i + 1] != line[i])
		{
			len = i;
			break ;
		}
		len = i;
	}
	return (len + 1);
}
