/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:20 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/06/22 12:59:32 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// "ysufu isalam 'abc "

void	quotes_remover(t_token *token)
{
	char	*new_val;
	int		len;
	char	q_flag;
	int		i;
	int		j;

	len = ft_strlen(token->value);
	new_val = malloc(sizeof(char) * len + 1);
	i = 0;
	j = 0;
	while (token->value[i])
	{
		if (token->value[i] == 39 || token->value[i] == 34)
		{
			if (q_flag == 0)
				q_flag = token->value[i];
			else if (q_flag == token->value[i])
				q_flag = 0;
		}
		else
			new_val[j++] = token->value[i];
		i++;
	}
	new_val[j] = '\0';
	if (new_val)
		free(token->value);
	if (new_val)
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
