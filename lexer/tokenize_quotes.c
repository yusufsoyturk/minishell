# include "../inc/minishell.h"

// zaten başlangıç değerim double quotes
int	double_quotes_len(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 34 && i != 0)
		{
			if (line[i + 1] == 34 || line[i + 1] == 39)
			{
				while (line[i + 1])
				{
					i++;
					if (line[i + 1] == 34)
						return (i + 2);
				}
			}
			break ;
		}
		i++;
	}
	return (i + 1);
}

//bu fonksiyondan döndürülen token freelenmeli
char	*in_double_quotes(char *line)
{
	char	*token;
	int len;
	int	i;

	i = 0;
	len = double_quotes_len(line);
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

int	single_quote_len(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 39 && i != 0)
		{
			if (line[i + 1] == 39 || line[i + 1])
			{
				while (line[i + 1])
				{
					i++;
					if (line[i + 1] == 39)
						return (i + 2);
				}
			}
			break ;
		}
		i++;
	}
	return (i + 1);
}

char	*in_single_quote(char *line)
{
	char	*token;
	int len;
	int	i;

	i = 0;
	len = single_quote_len(line);
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
