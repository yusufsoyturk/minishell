# include "../inc/minishell.h"

void	write_line(t_shell *minishell)
{
	char *line;
	line = minishell->line;
	printf("%s\n", line);
}