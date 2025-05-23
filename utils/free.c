#include "../inc/minishell.h"

void	free_double(t_shell *minishell)
{
	int	i;

	i = 0;
	while (minishell->args[i])
	{
		free(minishell->args[i]);
		i++;
	}
	free(minishell->args);
}

void	free_struct(t_shell *minishell)
{
	free(minishell->line);
	free_double(minishell);
	// free_token(minishell->token);
	free(minishell);
}

void	free_env_list(t_env *env_list)
{
    t_env *tmp;

    while (env_list)
    {
        tmp = env_list;
        env_list = env_list->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}
