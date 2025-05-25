# include "../inc/minishell.h"

void	error_message(t_shell *minishell, char *err_msg,t_env *env_list)
{
	(void)*env_list;
	(void)*minishell;
	(void)*err_msg;
	ft_putendl_fd(err_msg, 2);
}
// "merhaba 'yusuf" gibi örneklerde hata vermemeli
//, fonksiyonların içerisinde kullan genel kullanımı hatalı
void	missing_quotes_double(t_shell *minishell)
{
	int	i;
	int	quote_flag;

	i = 0;
	quote_flag = 0;
	while (minishell->line[i])
	{
		if ((minishell->line[i] == 34 || minishell->line[i] == 39) && quote_flag == 0)
		{
			if (minishell->line[i] == 34)
				quote_flag = 2;
			if (minishell->line[i] == 39)
				quote_flag = 1;
			i++;
		}
		if (minishell->line[i] == 34 && quote_flag == 2)
			quote_flag = 0;
		if (minishell->line[i] == 39 && quote_flag == 1)
			quote_flag = 0;
		i++;
	}
	if (quote_flag != 0)
		printf("Missing Quotes\n");
}


void	ft_tknadd_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
