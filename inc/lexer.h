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
	char *value;
	t_token_type type;
	struct s_token *next;
} t_token;

// tokenize_quotes.c
char	*in_double_quotes(char *line);
char	*in_single_quote(char *line);
// tokenize_func.c 
char	*new_word(char *line);

#endif