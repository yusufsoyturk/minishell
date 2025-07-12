#ifndef LEXER_H
# define LEXER_H

# include "../libft/libft.h"


typedef enum s_token_type{
	T_WORD, 
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC,
	T_ENV_VAR
} t_token_type;

typedef struct s_token
{
	char			*value;
	int				here_flag;
	t_token_type	type;
	struct s_token	*next;
} t_token;

// tokenize_quotes.c
int		double_quotes_len(char *line);
char	*in_double_quotes(char *line);
int		single_quote_len(char *line);
char	*in_single_quote(char *line);

// tokenize_control
int	word_len(char *line);

// tokenize_func.c
char	*new_word(char *line);
t_token_type	find_type(char *line);
char	*find_value(char *line, int *i);
t_token	*new_token(t_token *token, char *value);

// tokenize.c
t_token	*make_token(char *line, t_token *head);

// tokenize_utils.c
void	ft_tknadd_back(t_token **lst, t_token *new);


#endif