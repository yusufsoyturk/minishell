/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:17:20 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 19:06:28 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	error_message(t_shell *minishell, char *err_msg, t_env *env_list)
{
	(void)*env_list;
	(void)*minishell;
	(void)*err_msg;
	ft_putendl_fd(err_msg, 2);
}

static int	get_quote_flag(const char *line)
{
	int	i;
	int	quote_flag;

	i = 0;
	quote_flag = 0;
	while (line[i])
	{
		if (line[i] == 34)
		{
			if (quote_flag == 0)
				quote_flag = 2;
			else if (quote_flag == 2)
				quote_flag = 0;
		}
		else if (line[i] == 39)
		{
			if (quote_flag == 0)
				quote_flag = 1;
			else if (quote_flag == 1)
				quote_flag = 0;
		}
		i++;
	}
	return (quote_flag);
}

static void	cleanup_quotes_error(t_shell *minishell)
{
	printf("Missing Quotes\n");
	if (minishell->args)
		free_double(minishell);
	if (minishell->line)
		free(minishell->line);
}

int	missing_quotes_double(t_shell *minishell)
{
	int	quote_flag;

	quote_flag = get_quote_flag(minishell->line);
	if (quote_flag != 0)
	{
		cleanup_quotes_error(minishell);
		return (-1);
	}
	return (0);
}

void	ft_tknadd_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
