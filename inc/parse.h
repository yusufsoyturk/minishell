/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 09:42:07 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/13 09:42:25 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# define R_HEREDOC -42

# include "../libft/libft.h"
# include "minishell.h"
# include <stdlib.h>

typedef struct s_redir
{
	char				*target;
	int					flag;
	int					here_flag;
	int					fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	int					input;
	int					output;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

void		ft_free_tab(char **tab);
t_command	*pars(t_token *token, t_env *env, t_shell *mini);
int			token_check_pipe(char *str, t_shell *mini);
int			handle_redirection_parse(t_token **token,
				t_command *current, t_shell *mini);
t_command	*init_command(void);
int			token_check(char *str, t_shell *mini);
int			add_arg_to_command(t_command *cmd, const char *arg);

#endif