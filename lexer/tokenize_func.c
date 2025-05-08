# include "../inc/minishell.h"

// kelime boşluğa "" ve '' eşit deilse girecek
static int	word_len(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (line[i])
	{
		if (line[i] == 32)
			break;
		len = i;
		i++;
	}
	return (len + 1);
}

char	*new_word(char *line)
{
	char	*token;
	int len;
	int	i;

	i = 0;
	len = word_len(line);
	token = malloc(sizeof(char *) * len + 1);
	while (len > 0)
	{
		token[i] = line[i];
		i++;
		len--;
	}
	token[i] = '\0';
	return (token);
}