/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:20 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/06/21 19:43:09 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// "ysufu isalam 'abc "

char	*remove_double(t_token *token, int *i)
{
	int len;
	int a;
	char *new_val;
	int d_flag;

	d_flag = 0;
	a = 0;
	len = ft_strlen(token->value);
	new_val = malloc(sizeof(char) * len + 1);
	
	while (new_val[a] && token->value[*i])
	{
		if (token->value[*i] == 34 && d_flag == 0)
			d_flag = 1;
		else if (token->value[*i] == 34 && d_flag == 1)
			d_flag = 0;
		else
		{
			new_val[a] = token->value[*i];
			a++;
		}
		(*i)++;
	}
	new_val[a] = '\0';
	return(new_val);
}

char	*remove_single(t_token *token, int *i)
{
	int len;
	int a;
	char *new_val;
	int d_flag;

	d_flag = 0;
	a = 0;
	len = ft_strlen(token->value);
	new_val = malloc(sizeof(char) * len + 1);
	
	while (new_val[a] && token->value[*i])
	{
		if (token->value[*i] == 39 && d_flag == 0)
			d_flag = 1;
		else if (token->value[*i] == 39 && d_flag == 1)
			d_flag = 0;
		else
		{
			new_val[a] = token->value[*i];
			a++;
		}
		(*i)++;
	}
	new_val[a] = '\0';
	return(new_val);
}

char	*out_quotes(t_token *token, int *i)
{
	char	*new_val;
	int		a;

	a = 0;
	new_val = malloc(sizeof(char) * ft_strlen(token->value));
	while (new_val[a] && token->value[*i])
	{
		new_val[a] = token->value[*i];
		(*i)++;
		a++;
	}
	new_val[a] = '\0';
	return (new_val);
}

void	remove_quotes(t_token *token)
{
	t_token *head;
	int	*i;
	int a;
	char *tmp;
	char *tmp2;
	char *new_val;

	a = 0;
	i = &a;
	tmp = NULL;
	tmp2 = NULL;
	new_val = NULL;
	head = token;
	while (token)
	{
		(*i) = 0;
		if (tmp)
			free(tmp);
		tmp = malloc(sizeof(char) * ft_strlen(token->value) + 1);
		new_val = malloc(sizeof(char) * ft_strlen(token->value) + 1);
		while (token->value[*i])
		{
			if (token->value[*i] == 34)
				tmp = remove_double(token, i);
			else if (token->value[*i] == 39)
				tmp = remove_single(token, i);
			else
				tmp = out_quotes(token, i);
			if (!new_val)
				new_val = ft_strdup(tmp);
			else if (new_val)
			{
				tmp2 = ft_strdup(new_val);
				free(new_val);
				new_val = ft_strjoin(tmp2, tmp);
			}
			(*i)++;
		}
		if (token->value)
		{
			free(token->value);
			token->value = new_val;
		}
		// free(tmp);
		free(tmp2);
		token = token->next;
	}
}
