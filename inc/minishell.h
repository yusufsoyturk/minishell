#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/libft.h"
# include "lexer.h"
# include "parse.h"

typedef struct s_shell
{
	char	*line;
	char	**args;
	t_token	*token;
}	t_shell;

// tokenize_utils.c
void	error_message(t_shell *minishell, char *err_msg);
void	missing_quotes(t_shell *minishell);

// tokenize.c
int		ft_token(t_shell *minishell);

// utils utils.c
int		ft_atoll(const char *str);
// utils free.c
void	free_double(t_shell *minishell);
void	free_struct(t_shell *minishell);


void	exit_error(char *arg, char *err_msg, char *type);
void	built(char **args, char *input);
int		array_len(char **args);

void	free_token(t_token *token);

#endif