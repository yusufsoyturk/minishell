/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expand_str.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:59:10 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 00:17:42 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//char *expand_env_var_str(const char *str, t_env *env_list)
//{
//	char *result = NULL;
//	char *tmp2;
//	int  i = 0;

//	while (str[i])
//	{
//		if (str[i] == '\'')
//		{
//			int start = ++i;
//			while (str[i] && str[i] != '\'')
//				i++;
//			tmp2 = ft_substr(str, start, i - start);
//			result = append_str(result, tmp2);
//			free(tmp2);
//			if (str[i] == '\'')
//				i++;
//		}
//		else if (str[i] == '$')
//		{
//			if (str[i + 1] == '\0' || str[i + 1] == ' ' || str[i + 1] == '"' || str[i + 1] == '\'')
//			{
//				tmp2 = ft_strdup("$");
//				i++;
//			}
//			else
//			{
//				int start = ++i;
//				while (ft_isalnum(str[i]) || str[i] == '_')
//					i++;
//				char *var_name = ft_substr(str, start, i - start);
//				char *val = NULL;
//				t_env *cur = env_list;
//				while (cur)
//				{
//					if (ft_strcmp(cur->key, var_name) == 0)
//					{
//						val = ft_strdup(cur->value);
//						break;
//					}
//					cur = cur->next;
//				}
//				free(var_name);
//				tmp2 = val ? val : ft_strdup("");
//			}
//			result = append_str(result, tmp2);
//			free(tmp2);
//		}
//		else
//		{
//			tmp2 = ft_substr(str, i, 1);
//			result = append_str(result, tmp2);
//			free(tmp2);
//			i++;
//		}
//	}
//	if (!result)
//		result = ft_strdup("");
//	return result;
//}
