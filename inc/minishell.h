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
# include <sys/stat.h>

extern volatile sig_atomic_t g_sigint_received;

typedef struct	s_shell
{
	char	*line;
	char	**args;
	int		last_status;
	int		d_flag;
	t_token	*token;
}				t_shell;

//çevre değişkenlerini tutuyor
typedef struct s_env
{
	char	*key;
	char	*value;
	int		exported;
	struct s_env *next;
}				t_env;

typedef struct s_quote_ctx
{
    int	i;
    int	j;
}	t_quote_ctx;

typedef struct s_exec_ctx
{
	t_command	*current;
	t_env		**env_list;
	t_shell		*mini;
	t_command	*cmd;
	int			pipe_fd[2];
	int			prev_fd;
	int			status;
	int			loop;
}				t_exec_ctx;

typedef struct s_carry
{
	t_env	*env_list;
	t_shell	*mini;
}			t_carry;

// tokenize_utils.c
void	error_message(t_shell *minishell, char *err_msg,t_env *env_list);
int		missing_quotes_double(t_shell *minishell);

// tokenize.c
int		ft_token(t_shell *minishell);

// utils utils.c
int		ft_atoll(const char *str);
// utils free.c
void	free_less(t_shell *minishell, t_command *commands);
void	free_double(t_shell *minishell);
void	free_struct(t_shell *minishell);
void	free_env_list(t_env *env_list);
void	free_commands(t_command *cmd);
void	free_env(t_env *env);
// utils init.c
void init_env(char **env, t_env **env_list);
// utils signal.c
void	setup_signals(void);
void	setup_child_signals(void);
void	setup_heredoc_signals(void);
void	ignore_signals(void);

void	exit_error(char *arg, char *err_msg, char *type);
int		array_len(char **args);

//built
int		built(t_command *cmd, t_env **env, t_shell *minishell);
int		ft_strcmp(const char *s1, const char *s2);
int		builtin_unset(const char *key, t_env **env_list);
int		builtin_export(char **args, t_env **env_list, t_shell *mini);
int		builtin_exit(t_command *cmd, t_env *env_list, t_shell *minishell);
int		builtin_env(t_env *env_list);
t_env	*find_env_node(t_env *env, const char *key);
void	append_env(t_env **env_list, t_env *new);
t_env	**env_to_array(t_env *env);
void	sort_env_array(t_env **arr);
t_env	*create_env_node(char *key, char *value, int exported);
int		is_builtin(char **cmd);
void	update_or_add_env(char *arg, t_env **env_list);
void	handle_invalid_identifier(char *arg, t_shell *mini);
void	handle_valid_identifier(char *arg, t_env **env_list);
int		builtin_cd(char **args, t_shell *mini);
int		builtin_echo(char **args);
int		env_size(t_env *env);

//main.c
void	free_max(t_shell *minishell, t_env *env, t_command *cmd);

//execute
int		execute(t_command *cmd, t_env **env_list, t_shell *mini);
int		handle_redirection(t_command *cmd, t_env *env_list, t_shell *mini, t_command *free_cmd);
int		handle_heredoc(t_redir *redir, t_carry *carry, t_command *cmd, t_command *free_cmd);
int		check_permissions(const char *path);
char	*expand_string(const char *input, t_env *env_list, int last_status);
char	**env_to_envp_array(t_env *env_list);
char	*get_path(char *cmd, char **env);
char	*append_str(char *base, const char *suffix);
void	heredoc_child(t_carry *carry, int pfd[2], t_command *cmd, t_command *free_cmd);
void	handle_heredoc_sig(int pipefd[2]);
void	check_permissions_exec(char *path, t_command *cmd, char **env);

// expander env_var
char	*expand_env_var(t_env *env_list, t_token *token, int *i, t_shell *mini);
void	ft_expand(t_env *env_list, t_shell *mini);
char	*expand_pre_quo(t_env *env_list, t_token *token, int *i);
void	free_token(t_token *token);

// expander expande_change
// static char	*find_env_value(t_env *env_list, char *key, int key_len);
char	*expand_chance_env(t_env *env_list, t_token *token, int *i);

// expander expand_env_var
// static char	*handle_dollar(t_env *env_list, t_token *token, int *i);
// static char	*handle_single_quote(t_env *env_list, t_token *token, int *i);
// static char	*handle_normal_char(t_token *token, int *i);
char	*empty_handle(char *str);
char	*expand_env_var(t_env *env_list, t_token *token, int *i, t_shell *mini);

//expander expand_pre
// static int	get_pre_len(t_token *token, t_shell *mini, int i);
char	*expand_pre(t_env *env_list, t_token *token, int *i, t_shell *mini);
char	*expand_pre_quo(t_env *env_list, t_token *token, int *i);

// expander handle_dollar
void	append_and_free(char **new_val, char *tmp2);
int	is_special_dollar(t_token *token, int i);
int	is_dollar_question(t_token *token, int i);
int	is_double_dollar(t_token *token, int i);
void	handle_dollar_question(t_shell *mini, char **new_val, int *i);

// expander handle_dollar_env
void	handle_double_dollar(char **new_val, int *i);
void	handle_special_dollar(char **new_val, int *i);
void	handle_env_var(t_env *env_list, t_shell *mini, char **new_val, int *i);
void	handle_s_quote(t_env *env_list, t_shell *mini, char **new_val, int *i);
void	handle_normal(t_env *env_list, t_shell *mini, char **new_val, int *i);

// expander expand_quotes
char	*expand_with_quotes(t_env *env_list, t_shell *mini, int *i);
int		dollar_control(t_token *token);
int		quotes_controler(t_token *token);

// expander remove_quotes
void	remove_quotes(t_token *token);

#endif