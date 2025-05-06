#ifndef LEXER_H
# define LEXER_H

# include "../libft/libft.h"


typedef enum s_token_type
{
	T_PIPE;
	T_WORD;
	T_
}

typedef struct s_token
{
	char *value;
	struct s_token *next;
} t_token;



#endif