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
void	missing_quotes_double(t_shell *minishell, t_env *env_list)
{
	int	i;
	int	len;
	int count_double;

	count_double = 0;
	i = 0;
	len = 0;
	while (minishell->line[i])
	{
		count_double = 0;
		if (minishell->line[i] == 34)
		{
			len++;
			count_double++;
			i++;
			while (minishell->line[i] && minishell->line[i] != 34 && minishell->line[i + 1])
			{
				i++;
				len++;
			}
			if (minishell->line[i] == 34)
				count_double++;
		}
		i++;
	}
	if (count_double % 2 == 1)
		error_message(minishell , "Missing Quotes", env_list);
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
