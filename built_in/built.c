#include "../inc/minishell.h"

static void	builtin_cd(char **args)
{
	char	*cwd;
	char	**tmp;

	cwd = (char *)malloc(1024 * sizeof(char));
	if (cwd == NULL)
	{
		perror("malloc");
		return ;
	}
	if (args[1] == NULL)
	{
		free(cwd);
		chdir(getenv("HOME"));
		return ;
	}
	if (ft_strncmp(args[1], "..", 2) == 0)
	{
		getcwd(cwd, 1024);
		tmp = ft_split(cwd, '/');
		if (tmp[0] == NULL)
			chdir("/");
		else
			chdir("..");
		ft_free_tab(tmp);
		free(cwd);
	}
	else if (chdir(args[1]) == -1)
	{
		if (ft_strncmp(args[1], ".", 1) == 0)
			return ;
		exit_error(args[1], "No such file or directory", "cd");
	}
	free(cwd);
}
static void	builtin_pwd(void)
{
	char	*cwd;

	cwd = (char *)malloc(1024 * sizeof(char));
	if (cwd == NULL)
	{
		perror("malloc");
		return ;
	}
	if (getcwd(cwd, 1024) == NULL)
	{
		perror("getcwd");
		free(cwd);
		return ;
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
}

static int	builtin_exit(char **args, char *input)
{
	int			i;
	long long	exit_code;

	i = -1;
	exit_code = 0;
	write(2, "exit\n", 5);
	if (args[1])
	{
		while (args[1][++i])
		{
			if (ft_isdigit(args[1][i]) == 0)
			{
				exit_error(args[1], "numeric argument required", "exit");
				return (1);
			}
		}
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
		exit_code = ft_atoll(args[1]) % 256;
	}
	ft_free_tab(args);
	free(input);
	exit(exit_code);
}

void	built(char **args, char *input, t_env **env)
{
	if (args[0] && ft_strncmp(args[0], "exit", 4) == 0 && args[0][4] == '\0')
		builtin_exit(args, input);
	else if (args[0] && ft_strncmp(args[0], "cd", 2) == 0 && args[0][2] == '\0')
		builtin_cd(args);
	else if (args[0] && ft_strncmp(args[0], "env", 3) == 0 && args[0][3] == '\0')
		builtin_env((*env));
	else if (args[0] && ft_strncmp(args[0], "export", 6) == 0 && args[0][6] == '\0')
		builtin_export(args, env);
	else if (args[0] && ft_strncmp(args[0], "unset", 5) == 0 && args[0][5] == '\0')
		builtin_unset(args[1], env);
	else if (args[0] && ft_strncmp(args[0], "pwd", 3) == 0 && args[0][3] == '\0')
		builtin_pwd();
}
