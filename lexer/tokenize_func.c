# include "../inc/minishell.h"

// kelime boşluğa "" ve '' eşit deilse girecek
int	word_len(char *line)
{
	int	i;
	int	len;
	int a;

	i = 0;
	a = 0;
	len = 0;
	while (line[i])
	{
		if (line[i] == 34)
		{
			a = i;
			i++;
			while (line[i] != 34)
				i++;
		}
		if (line[i] == 39)
		{
			a = i;
			i++;
			while (line[i] != 39)
				i++;
		}
		if (line[i] == 32  || (line[i] >= 9 && line[i] <= 13))		
			break;
		if (i > 0 && (line[i] == '>' || line[i] == '<' || line[i] == '|'))
		{
			if (line[i] != line[i - 1])
				break;
		}
		if (line[i] == '<' || line[i] == '>' || line[i] == '|')
		{
			if (line[i + 1] && (line[i + 1] != line[i]))
			{
				len = i;
				break;
			}
		}
		len = i;
		i++;
	}
	printf("%d\n", i);
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

t_token_type	find_type(char *line)
{
	if (ft_strncmp(line, "|", 1) == 0)
		return	T_PIPE;
	else if (ft_strncmp(line, ">>", 2) == 0)
		return	T_REDIR_APPEND;
	else if (ft_strncmp(line, "<<", 2) == 0)
		return	T_REDIR_HEREDOC;
	else if (ft_strncmp(line, "<", 1) == 0)
		return	T_REDIR_IN;
	else if (ft_strncmp(line, ">", 1) == 0)
		return	T_REDIR_OUT;
	else if (ft_strncmp(line, "$", 1) == 0)
		return	T_ENV_VAR;
	else
		return T_WORD;
}

// sadece boşluk varsa argüman sanıyor bu fonksiyonu kullanırken dikkat edilmeli
char	*find_value(char *line, int *i)
{
	char	*token;

	if (line[*i] == 34)
	{
		token = in_double_quotes(line + (*i));
		*i += double_quotes_len(token);
		// printf("%d %s\n", *i, token);
	}
	else if (line[*i] == 39)
	{
		token = in_single_quote(line + *i);
		*i += single_quote_len(token);
	}
	else
	{
		token = new_word(line + *i);
		*i += word_len(token);
	}
	return (token);
}

t_token	*new_token(t_token *token, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->type = find_type(value);
	new->value = ft_strdup(value);
	new->next = NULL;
	ft_tknadd_back(&token, new);
	return (token);
}

