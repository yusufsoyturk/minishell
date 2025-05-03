#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include "lexer.h"
# include "parse.h"

// char	token_maker(char *input);
int		ft_atoll(const char *str);
void	exit_error(char *arg, char *err_msg, char *type);
void	built(char **args, char *input);
int		array_len(char **args);

#endif