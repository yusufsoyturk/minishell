/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoyturk <ysoyturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 09:40:38 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/17 15:22:53 by ysoyturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L
# include "../libft/libft.h"
# include "lexer.h"
# include "parse.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_sigint_received;

typedef struct s_shell
{
	char						*line;
	char						**args;
	int							last_status;
	int							d_flag;
	t_token						*token;
}								t_shell;

typedef struct s_env
{
	char						*key;
	char						*value;
	int							exported;
	struct s_env				*next;
}								t_env;

typedef struct s_quote_ctx
{
	int							i;
	int							j;
}								t_quote_ctx;

typedef struct s_exec_ctx
{
	t_command					*current;
	t_env						**env_list;
	t_shell						*mini;
	t_command					*cmd;
	int							pipe_fd[2];
	pid_t						pids[1024];
	int							redir_exit;
	int							pid_count;
	int							prev_fd;
	int							status;
	int							loop;
}								t_exec_ctx;

typedef struct s_carry
{
	t_env						*env_list;
	t_shell						*mini;
}								t_carry;

void	error_message(t_shell *minishell, char *err_msg,
			t_env *env_list);
int		missing_quotes_double(t_shell *minishell);
int		ft_token(t_shell *minishell);
int		ft_atoll(const char *str);
void	free_less(t_shell *minishell,
			t_command *commands);
void	free_double(char **arr);
void	free_struct(t_shell *minishell);
void	free_env_list(t_env *env_list);
void	free_commands(t_command *cmd);
void	free_env(t_env *env);
void	init_env(char **env, t_env **env_list);
void	setup_signals(void);
void	setup_child_signals(void);
void	setup_heredoc_signals(void);
void	free_redirections(t_redir *redir);
void	ignore_signals(void);
void	exit_error(char *arg, char *err_msg,
			char *type);
int		handle_fd_error(t_redir *r, t_shell *mini, t_carry *carry);
int		array_len(char **args);
int		built(t_command *cmd, t_env **env,
			t_shell *minishell, t_command *current);
void	init_exec_ctx(t_exec_ctx *ctx, t_command *cmd,
			t_env **env_list, t_shell *mini);
int		ft_strcmp(const char *s1, const char *s2);
int		builtin_unset(const char *key,
			t_env **env_list);
int		builtin_export(char **args, t_env **env_list,
			t_shell *mini);
int		builtin_exit(t_command *cmd, t_env *env_list,
			t_shell *minishell, t_command *current);
int		builtin_env(t_env *env_list);
t_env	*find_env_node(t_env *env, const char *key);
int		handle_child_redirection(t_exec_ctx *ctx);
void	append_env(t_env **env_list, t_env *new);
t_env	**env_to_array(t_env *env);
int		handle_redirection_error_exec(t_exec_ctx *ctx);
void	sort_env_array(t_env **arr);
t_env	*create_env_node(char *key, char *value,
			int exported);
int		is_builtin(char **cmd);
void	update_or_add_env(char *arg, t_env **env_list);
void	handle_invalid_identifier(char *arg,
			t_shell *mini);
void	handle_valid_identifier(char *arg,
			t_env **env_list);
int		builtin_cd(char **args, t_shell *mini);
int		builtin_echo(char **args);
int		env_size(t_env *env);
void	free_max(t_shell *minishell, t_env *env,
			t_command *cmd);
int		execute(t_command *cmd, t_env **env_list,
			t_shell *mini);
int		handle_redirection_exec(t_command *cmd,
			t_env *env_list, t_shell *mini,
			t_command *free_cmd);
int		handle_heredoc(t_redir *redir, t_carry *carry, t_command *free_cmd);
int		check_permissions(const char *path);
char	*expand_string(const char *input,
			t_env *env_list, int last_status);
char	**env_to_envp_array(t_env *env_list);
char	*get_path(char *cmd, char **env);
char	*append_str(char *base, const char *suffix);
void	heredoc_child(t_carry *carry, int pfd[2],
			t_redir *redir, t_command *free_cmd);
void	handle_heredoc_sig(int pipefd[2]);
void	check_permissions_exec(char *path,
			t_command *cmd, char **env);
void	child_builtin_exit(t_exec_ctx *ctx);
void	setup_child_io(t_exec_ctx *ctx);
void	child_cmd_not_found(t_exec_ctx *ctx);
void	child_process(t_exec_ctx *ctx);
void	handle_builtin_or_exec(t_exec_ctx *ctx);
void	child_execve(t_exec_ctx *ctx, char **char_env,
			char *exec_path);
char	*expand_env_var(t_env *env_list, t_token *token,
			int *i, t_shell *mini);
void	ft_expand(t_env *env_list, t_shell *mini);
char	*expand_pre_quo(t_env *env_list, t_token *token,
			int *i);
void	free_token(t_token *token);
char	*expand_chance_env(t_env *env_list,
			t_token *token, int *i);
char	*empty_handle(char *str);
char	*expand_env_var(t_env *env_list, t_token *token,
			int *i, t_shell *mini);
char	*expand_pre(t_env *env_list, t_token *token,
			int *i, t_shell *mini);
char	*expand_pre_quo(t_env *env_list, t_token *token,
			int *i);
void	append_and_free(char **new_val, char *tmp2);
int		is_special_dollar(t_token *token, int i);
int		is_dollar_question(t_token *token, int i);
int		is_double_dollar(t_token *token, int i);
void	handle_dollar_question(t_shell *mini,
			char **new_val, int *i);
void	handle_double_dollar(char **new_val, int *i);
void	handle_special_dollar(char **new_val, int *i);
void	handle_env_var(t_env *env_list, t_shell *mini,
			char **new_val, int *i);
void	handle_s_quote(t_env *env_list, t_shell *mini,
			char **new_val, int *i);
void	handle_normal(t_env *env_list, t_shell *mini,
			char **new_val, int *i);
char	*expand_with_quotes(t_env *env_list,
			t_shell *mini, int *i);
int		dollar_control(t_token *token);
int		quotes_controler(t_token *token);
void	remove_quotes(t_token *token);
char	*handle_dollar_question_quo(t_shell *mini, char **new_val, int *i);
int		get_pre_len(t_token *token, t_shell *mini, int i);
char	*handle_dollar(t_env *env_list, t_token *token, int *i);
char	*handle_single_quote(t_env *env_list, t_token *token, int *i);
char	*handle_normal_char(t_token *token, t_shell *mini, int *i);

#endif