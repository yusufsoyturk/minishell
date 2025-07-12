/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 16:46:53 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 16:49:27 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_double_dollar(char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = ft_strdup("$");
	append_and_free(new_val, tmp2);
	*i += 2;
}

void	handle_special_dollar(char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = ft_strdup("$");
	append_and_free(new_val, tmp2);
	(*i)++;
}

void	handle_env_var(t_env *env_list, t_shell *mini, char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = expand_env_var(env_list, mini->token, i, mini);
	append_and_free(new_val, tmp2);
}

void	handle_s_quote(t_env *env_list, t_shell *mini, char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = expand_pre_quo(env_list, mini->token, i);
	append_and_free(new_val, tmp2);
}

void	handle_normal(t_env *env_list, t_shell *mini, char **new_val, int *i)
{
	char	*tmp2;

	tmp2 = expand_pre(env_list, mini->token, i, mini);
	append_and_free(new_val, tmp2);
}
