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
//PRİNT ALANI

void print_tokens(t_token *tokens) 
{
    while (tokens)
	{
        printf("Type = %s, Value = '%s'\n", token_type_to_string(tokens->type), tokens->value);
		tokens = tokens->next;
	}
}

void print_redirections(t_redir *redir)
{
    while (redir)
    {
        printf("    🔁 Redirection: target='%s', flags=%d, fd=%d\n",
               redir->target, redir->flag, redir->fd);
        redir = redir->next;
    }
}

void print_commands(t_command *cmds)
{
    int i = 1;
    while (cmds)
    {
        printf("=== Command %d ===\n", i);

        // Argümanları yazdır
        if (cmds->args)
        {
            printf("  🔹 Args: ");
            for (int j = 0; cmds->args[j]; j++)
                printf("'%s' ", cmds->args[j]);
            printf("\n");
        }
        else
            printf("  🔹 Args: (none)\n");

        // Giriş ve çıkış dosya tanımlayıcıları
        printf("  🔸 Input FD: %d\n", cmds->input);
        printf("  🔸 Output FD: %d\n", cmds->output);

        // Redirection varsa
        if (cmds->redirs)
            print_redirections(cmds->redirs);
        else
            printf("  🔁 No redirections.\n");

        printf("\n");
        cmds = cmds->next;
        i++;
    }
}

// PRİNT ALANI CAN GPT BABAYA SELAMLAR

volatile sig_atomic_t g_sigint = 0;

void	free_max(t_shell *minishell, t_env *env, t_command *cmd)
{
	free_env(env);
	free_double(minishell);
	free_token(minishell->token);
	free(minishell->line);
	free(minishell);
	free_commands(cmd);
}

void	free_less(t_shell *minishell, t_command *commands)
{
    free_double(minishell);
    free_token(minishell->token);
    free(minishell->line);
    free_commands(commands);
}


int main(int ac, char **av, char **env)
{
	t_shell				*minishell;
	t_env				*env_list;
	t_command			*commands;

	minishell = malloc(sizeof(t_shell));
	setup_signals();
	env_list = NULL;
	ft_init_shell(minishell);
	init_env(env, &env_list);
	if (ac == 1 && av[0])
	{
		while (1)
		{
			minishell->line = readline("minishell> ");
			if (minishell->line && missing_quotes_double(minishell) == 0)
			{
				if (!minishell->line)
					minishell->line = ft_strdup("exit");
				else
					add_history(minishell->line);
				ft_token(minishell);
				ft_expand(env_list, minishell);
				print_tokens(minishell->token);
				minishell->args = ft_split(minishell->line, ' ');
				commands = pars(minishell->token, env_list);
				// print_commands(commands);
				if (commands && (commands->args || commands->redirs))
					execute(commands, &env_list, env, minishell);
				free_less(minishell, commands);
			}

		}
		free_struct(minishell);
	}
	else 
		error_message(minishell, "Invalid arguments", env_list);
	return (0);
}
