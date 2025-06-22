# include "../inc/minishell.h"

// zaten başlangıç değerim double quotes
int	double_quotes_len(char *line)
{
	int	i;
	int flag;

	flag = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == 34)
		{
			flag++;
			if (line[i + 1] == 34 || line[i + 1] == 39)
			{
				while (line[i + 1])
				{
					i++;
					if (line[i + 1] == 34 && ((line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == 32) ))
					{
						return (i + 2);
					}
				}
			}
		}
		else if ((line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == 32) && flag > 1)
		{
			printf("deneme %d\n", i);
			break;
		}
		i++;
	}
	return (i);
}

//bu fonksiyondan döndürülen token freelenmeliMore actions
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
	int flag;

	flag = 0;
	i = 0;
	while (line[i])
	{
		printf("%c\n", line[i]);
		if (line[i] == 39)
		{
			flag++;
			if (line[i + 1] == 39)
			{
				while (line[i + 1])
				{
					i++;
					if (line[i + 1] == 39)
						return (i + 2);
				}
			}
		}
		else if ((line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == 32) && flag > 1)
			break;
		i++;
	}
	return (i);
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
