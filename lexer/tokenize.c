# include "../inc/minishell.h"

void	make_token(char *line, t_token *head)
{
	int		j;
	int		*i;
	char	*token;

	j = 0;
	i = &j;
	while (line[*i])
	{
		if (line[*i] == 32)
			(*i)++;
		else
		{
			token = find_value(line, i);
			head = new_token(head, token);
			free(token);
		}
	}
}

int	ft_token(t_shell *minishell)
{
	char	*line;
	t_token	*token_head;

	token_head = NULL;
	token_head = malloc(sizeof(t_token));
	line = minishell->line;
	make_token(line, token_head);
	return 0;
}