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

void write_token(t_shell *minishell)
{
	while (minishell->token)
	{
		printf("%s %u\n", minishell->token->value, minishell->token->type);

		minishell->token = minishell->token->next;
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
			ft_token(minishell);
			// write_token(minishell);
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
