/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 16:37:49 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 16:45:41 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	append_and_free(char **new_val, char *tmp2)
{
	char	*tmp;

	if (!*new_val)
		*new_val = tmp2;
	else
	{
		tmp = ft_strjoin(*new_val, tmp2);
		free(*new_val);
		*new_val = tmp;
		free(tmp2);
	}
}

int	is_special_dollar(t_token *token, int i)
{
	return (token->value[i] == '$' && (token->value[i + 1] == '\0'
			|| token->value[i + 1] == ' ' || token->value[i + 1] == '"'
			|| token->value[i + 1] == '$'));
}

int	is_dollar_question(t_token *token, int i)
{
	return (ft_strncmp(token->value + i, "$?", 2) == 0);
}

int	is_double_dollar(t_token *token, int i)
{
	return (ft_strncmp(token->value + i, "$", 2) == 0);
}

void	handle_dollar_question(t_shell *mini, char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = ft_itoa(mini->last_status);
	append_and_free(new_val, tmp2);
	*i += 2;
}
