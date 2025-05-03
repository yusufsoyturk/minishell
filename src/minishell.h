#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>

// char	token_maker(char *input);
void	ft_free_tab(char **tab);
char	*pars(char *input, char **env);
int		ft_atoll(const char *str);
void	exit_error(char *arg, char *err_msg, char *type);
void	built(char **args, char *input);
int		array_len(char **args);

#endif