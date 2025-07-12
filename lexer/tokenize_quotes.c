/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:17:13 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 21:05:16 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	double_quotes_len(char *line)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line[i])
	{
		if (line[i] == 34)
		{
			flag++;
			if (line[i + 1] == 34 || line[i + 1] == 39)
				flag = 0;
		}
		else if ((line[i] == '<' || line[i] == '>' || line[i] == '|'
				|| line[i] == 32) && flag > 1)
			break ;
		i++;
	}
	return (i);
}

char	*in_double_quotes(char *line)
{
	char	*token;
	int		len;
	int		i;

	i = 0;
	len = double_quotes_len(line);
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

int	single_quote_len(char *line)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line[i])
	{
		if (line[i] == 39)
		{
			flag++;
			if (line[i + 1] == 34 || line[i + 1] == 39)
				flag = 0;
		}
		else if ((line[i] == '<' || line[i] == '>' || line[i] == '|'
				|| line[i] == 32) && flag > 1)
			break ;
		i++;
	}
	return (i);
}

char	*in_single_quote(char *line)
{
	char	*token;
	int		len;
	int		i;

	i = 0;
	len = single_quote_len(line);
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
