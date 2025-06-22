# include "../inc/minishell.h"

t_token	*make_token(char *line, t_token *head)
{
	int		j;
	int		*i;
	char	*token;
	// int		is_open = -1;

	j = 0;
	i = &j;
	while (line[*i])
	{
		// if (is_open == -1 && (line[*i] == '\'' || line[*i] == '\"')) {
		// 	is_open = line[*i];
		// } else if (is_open != -1 && (is_open == line[*i])) {
		// 	is_open = -1;
		// }
		if ((line[*i] == 32 || (line[*i] >= 9 && line[*i] <= 13)))
			(*i)++;
		else
		{
			token = find_value(line, i);
			head = new_token(head, token);
			free(token);
		}
	}
	return (head);
}

int	ft_token(t_shell *minishell)
{
	char	*line;

	minishell->token = NULL;
	line = minishell->line;
	minishell->token = make_token(line, minishell->token);
	return (0);
}