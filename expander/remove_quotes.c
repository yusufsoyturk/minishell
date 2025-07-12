/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:55:20 by ysoyturk          #+#    #+#             */
/*   Updated: 2025/07/12 17:13:10 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	copy_without_quote(char *src, char *dst, t_quote_ctx *ctx,
		char quote)
{
	ctx->i++;
	while (src[ctx->i] && src[ctx->i] != quote)
	{
		dst[ctx->j] = src[ctx->i];
		ctx->j++;
		ctx->i++;
	}
	if (src[ctx->i] == quote)
		ctx->i++;
}

void	quotes_remover(t_token *token)
{
	char		*new_val;
	int			len;
	t_quote_ctx	ctx;

	len = ft_strlen(token->value);
	new_val = malloc(sizeof(char) * (len + 1));
	if (!new_val)
		return ;
	ctx.i = 0;
	ctx.j = 0;
	while (token->value[ctx.i])
	{
		if (token->value[ctx.i] == 34)
			copy_without_quote(token->value, new_val, &ctx, 34);
		else if (token->value[ctx.i] == 39)
			copy_without_quote(token->value, new_val, &ctx, 39);
		else
			new_val[ctx.j++] = token->value[ctx.i++];
	}
	new_val[ctx.j] = '\0';
	free(token->value);
	token->value = new_val;
}

void	remove_quotes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if (curr->value)
			quotes_remover(curr);
		curr = curr->next;
	}
}
