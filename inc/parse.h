#ifndef PARSE_H
# define PARSE_H

# define R_HEREDOC -42

#include <stdlib.h>
# include "../libft/libft.h"
# include "minishell.h"

//redirection tutan struct
typedef struct	s_redir
{
	char			*target; //yönlendirilen dosyanın adı
	int				flag;    //sahip olacağı flagler örn O_APPEND, O_TRUNC türüne göre değişiyor
	int				here_flag;
	int				fd;		 //kullanılan dosya ama o kısım execute'ta açılacak ilk pars kısmında -1 olacak
	struct s_redir	*next;   //bir komut kısmında birden fazla redir varsa
}				t_redir;

typedef struct	s_command //bu kısım t_shell'le bağlantılı olacak
{
	char				**args;  //argümanlar
	int					input;	 //execute'da değişecek
	int					output;  //execute'da değişecek
	t_redir				*redirs; 
	struct s_command	*next;
}				t_command;

//böyle bir şey yapmam gerekti
typedef struct s_env t_env;
typedef struct s_shell t_shell;

// parse_utils.c
void	ft_free_tab(char **tab);
// parser.c
t_command	*pars(t_token *token, t_env *env, t_shell *mini);
int	token_check_pipe(char *str, t_shell *mini);
int	handle_redirection_parse(t_token **token, t_command *current, t_shell *mini);
t_command	*init_command(void);
int	token_check(char *str, t_shell *mini);
int	add_arg_to_command(t_command *cmd, const char *arg);

#endif