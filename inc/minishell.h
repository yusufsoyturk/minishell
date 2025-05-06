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
# include <stdio.h>

typedef struct s_shell
{
    char *line;
    char **args;
}   t_shell;

// tokenize.c
void	write_line(t_shell *minishell);

// utils utils.c
int		ft_atoll(const char *str);
// utils free.c
void	free_double(t_shell *minishell);
void	free_struct(t_shell *minishell);

void	exit_error(char *arg, char *err_msg, char *type);
void	built(char **args, char *input);
int		array_len(char **args);

#endif