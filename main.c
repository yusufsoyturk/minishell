#include "inc/minishell.h"

void    write_args(char **args)
{
	int i;
	i = 0;
	while (args[i])
	{
		printf("%s\n", args[i]);
		i++;
	}
}

static void ft_init_shell(t_shell *minishell)
{
	ft_memset(minishell, 0, sizeof(t_shell));
}

const char *token_type_to_string(t_token_type type) {
    switch (type) {
        case T_WORD: return "T_WORD";
        case T_PIPE: return "T_PIPE";
        case T_REDIR_IN: return "T_REDIR_IN";
        case T_REDIR_OUT: return "T_REDIR_OUT";
        case T_REDIR_APPEND: return "T_REDIR_APPEND";
        case T_REDIR_HEREDOC: return "T_REDIR_HEREDOC";
        case T_ENV_VAR: return "T_ENV_VAR";
        default: return "T_UNKNOWN";
    }
}

// Token'ları yazdıran fonksiyon
void print_tokens(t_token *tokens) 
{
    while (tokens)
	{
        printf("Type = %s, Value = '%s'\n", token_type_to_string(tokens->type), tokens->value);
		tokens = tokens->next;
	}
}

void	free_token(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

int main(int ac, char **av, char **env)
{
	t_shell *minishell;
	t_env    *env_list;

	minishell = malloc(sizeof(t_shell));
	env_list = NULL;
	ft_init_shell(minishell);
	init_env(env, &env_list);
	if (ac == 1 && av[0])
	{
		while (1)
		{
			minishell->line = readline("minishell> ");
			missing_quotes_double(minishell);
			ft_token(minishell);
			print_tokens(minishell->token);
			minishell->args = ft_split(minishell->line, ' ');
			built(minishell, &env_list);
			// pars(input, env);
			//execute
			free_double(minishell);
			free_token(minishell->token);
			free(minishell->line);
		}
		free_struct(minishell);
	}
	else 
		error_message(minishell, "Invalid arguments");
	return (0);
}
