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
}

void	free_struct(t_shell *minishell)
{
	ft_memset(minishell, 0, sizeof(t_shell));
	free(minishell);
}