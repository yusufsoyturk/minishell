# include "../inc/minishell.h"

// zaten başlangıç değerim double quotes
static int	double_quotes_len(char *line)
{
	int	i;
	int	end;

	i = 0;
	end = 0;
	while (line[i])
	{
		if (line[i] == 34 && i != 0)
		{
			end = i;
			break ;
		}
		i++;
	}
	return (end + 1);
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

static int	single_quote_len(char *line)
{
	int	i;
	int	end;

	i = 0;
	end = 0;
	while (line[i])
	{
		if (line[i] == 39 && i != 0)
		{
			end = i;
			break ;
		}
		i++;
	}
	return (end + 1);
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
