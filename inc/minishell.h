#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
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

//çevre değişkenlerini tutuyor
typedef struct s_env
{
	char	*key;
	char	*value;
	int		exported;
	struct s_env *next;
}				t_env;

// tokenize_utils.c
void	error_message(t_shell *minishell, char *err_msg,t_env *env_list);
void	missing_quotes_double(t_shell *minishell);

// tokenize.c
int		ft_token(t_shell *minishell);

// utils utils.c
int		ft_atoll(const char *str);
// utils free.c
void	free_double(t_shell *minishell);
void	free_struct(t_shell *minishell);
void	free_env_list(t_env *env_list);
void	free_commands(t_command *cmd);
// utils init.c
void init_env(char **env, t_env **env_list);
// utils signal.c
void	signal_handler(int sig);
void	setup_signals(void);
void setup_heredoc_signals(void);



void	exit_error(char *arg, char *err_msg, char *type);
int		array_len(char **args);

//built
int		built(t_command *cmd, t_env **env, t_shell *minishell);
int		ft_strcmp(const char *s1, const char *s2);
int		builtin_unset(const char *key, t_env **env_list);
int		builtin_export(char **args, t_env **env_list);
int		builtin_exit(t_command *cmd, t_env *env_list, t_shell *minishell);
int		builtin_env(t_env *env_list);
t_env	*find_env_node(t_env *env, const char *key);
void	append_env(t_env **env_list, t_env *new);
t_env	**env_to_array(t_env *env);
void	sort_env_array(t_env **arr);
int		is_builtin(char **cmd);
int		builtin_echo(char **args);

//main.c
void	free_max(t_shell *minishell, t_env *env, t_command *cmd);

//execute
int	execute(t_command *cmd, t_env **env_list, char **env, t_shell *mini);


// expander env_var
char *expand_env_var(t_env *env_list, t_token *token, int *i);
void	ft_expand(t_env *env_list, t_token *token);

void	free_token(t_token *token);

#endif