#include "../inc/minishell.h"

void	exit_error(char *arg, char *err_msg, char *type)
{
	char	*tmp;
	char	*tmp2;
	
	if (ft_strncmp(type, "exit", 4) == 0)
		tmp = ft_strjoin("minishell: exit: ", arg);
	else if (ft_strncmp(type, "cd", 2) == 0)
		tmp = ft_strjoin("minishell: cd: ", arg);
	else if (ft_strncmp(type, "env", 3) == 0)
		tmp = ft_strjoin("minishell: env: ", arg);
	else
		tmp = ft_strjoin("minishell: ", arg);
	tmp2 = ft_strjoin(tmp, ": ");
	free(tmp);
	tmp = ft_strjoin(tmp2, err_msg);
	free(tmp2);
	ft_putendl_fd(tmp, 2);
	free(tmp);
}

int	array_len(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}