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

int	builtin_exit(t_shell *minishell, t_env *env_list)
{
	int			i;
	long long	exit_code;

	i = -1;
	exit_code = 0;
	write(2, "exit\n", 5);
	if (minishell->args[1])
	{
		while (minishell->args[1][++i])
		{
			if (ft_isdigit(minishell->args[1][i]) == 0)
			{
				exit_error(minishell->args[1], "numeric argument required", "exit");
				return (1);
			}
		}
		if (minishell->args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
		exit_code = ft_atoll(minishell->args[1]) % 256;
	}
	free_token(minishell->token);
	free_struct(minishell);
	free_env_list(env_list);
	exit(exit_code);
}

void	built(t_shell *minishell, t_env **env)
{
	if (minishell->args[0] && ft_strncmp(minishell->args[0], "exit", 4) == 0 && minishell->args[0][4] == '\0')
		builtin_exit(minishell, (*env));
	else if (minishell->args[0] && ft_strncmp(minishell->args[0], "cd", 2) == 0 && minishell->args[0][2] == '\0')
		builtin_cd(minishell->args);
	else if (minishell->args[0] && ft_strncmp(minishell->args[0], "env", 3) == 0 && minishell->args[0][3] == '\0')
		builtin_env((*env));
	else if (minishell->args[0] && ft_strncmp(minishell->args[0], "export", 6) == 0 && minishell->args[0][6] == '\0')
		builtin_export(minishell->args, env);
	else if (minishell->args[0] && ft_strncmp(minishell->args[0], "unset", 5) == 0 && minishell->args[0][5] == '\0')
		builtin_unset(minishell->args[1], env);
	else if (minishell->args[0] && ft_strncmp(minishell->args[0], "pwd", 3) == 0 && minishell->args[0][3] == '\0')
		builtin_pwd();
}
