# include "../inc/minishell.h"

void	error_message(t_shell *minishell, char *err_msg)
{
	if (minishell != NULL)
		free_struct(minishell);
	ft_putendl_fd(err_msg, 2);
	exit(EXIT_FAILURE);
}
// "merhaba 'yusuf" gibi örneklerde hata vermemeli
//, fonksiyonların içerisinde kullan genel kullanımı hatalı
void	missing_quotes(t_shell *minishell)
{
	int i;
	int count_double;
	int count_single;

	i = 0;
	count_double = 0;
	count_single = 0;
	while (minishell->line[i])
	{
		if (minishell->line[i] == 34)
			count_double++;
		if (minishell->line[i] == 39)
			count_single++;
		i++;
	}
	if ((count_double % 2 != 0) || (count_single % 2 != 0))
		error_message(minishell, "Missing Quote");
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
