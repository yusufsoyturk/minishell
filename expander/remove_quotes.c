/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:20 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/06/30 22:07:10 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// " ' a"

void	quotes_remover(t_token *token)
{
	char	*new_val;
	int		len;
	int		q_flag;
	int		i;
	int		j;

	len = ft_strlen(token->value);
	new_val = malloc(sizeof(char) * len + 1);
	i = 0;
	j = 0;
	q_flag = 0;
	while (token->value[i])
	{
		if (token->value[i] == 34)
		{
			i++;
			while (token->value[i] && token->value[i] != 34)
				new_val[j++] = token->value[i++];
			i++;
		}
		else if (token->value[i] == 39)
		{
			i++;
			while (token->value[i] && token->value[i] != 39)
				new_val[j++] = token->value[i++];
			i++;
		}
		else
			new_val[j++] = token->value[i++];
	}
	new_val[j] = '\0';
	free(token->value);
	token->value = new_val;
}

void	remove_quotes(t_token *token)
{
	t_token *head;

	head = token;
	while (head)
	{
		if (head->value)
			quotes_remover(head);
		head = head->next;
	}
}
