#include "../inc/minishell.h"

static int	builtin_cd(char **args)
{
	char	*cwd;
	char	**tmp;

	cwd = (char *)malloc(1024 * sizeof(char));
	if (cwd == NULL)
	{
		perror("malloc");
		return (1);
	}
	if (args[1] == NULL)
	{
		free(cwd);
		chdir(getenv("HOME"));
		return (0);
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
	}
	else if (chdir(args[1]) == -1)
	{
		if (ft_strncmp(args[1], ".", 1) == 0)
		{
			free(cwd);
			return (0);
		}
		exit_error(args[1], "No such file or directory", "cd");
		free(cwd);
		return (1);
	}
	free(cwd);
	return (0);
}
static int	builtin_pwd(void)
{
	char	*cwd;

	cwd = (char *)malloc(1024 * sizeof(char));
	if (cwd == NULL)
	{
		perror("malloc");
		return (1);
	}
	if (getcwd(cwd, 1024) == NULL)
	{
		perror("getcwd");
		free(cwd);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}

int	builtin_exit(t_command *cmd, t_env *env_list, t_shell *minishell)
{
	int			i;
	long long	exit_code;

	i = -1;
	exit_code = 0;
	write(2, "exit\n", 5);
	if (cmd->args[1])
	{
		while (cmd->args[1][++i])
		{
			if (ft_isdigit(cmd->args[1][i]) == 0)
			{
				exit_error(cmd->args[1], "numeric argument required", "exit");
				return (1);
			}
		}
		if (cmd->args[2])
		{
			ft_putendl_fd("cmd: exit: too many arguments", 2);
			return (1);
		}
		exit_code = ft_atoll(cmd->args[1]) % 256;
	}
	free_max(minishell, env_list, cmd);
	exit(exit_code);
}

int	built(t_command *cmd, t_env **env, t_shell *minishell)
{
	if (cmd->args[0] && ft_strncmp(cmd->args[0], "exit", 4) == 0 && cmd->args[0][4] == '\0')
		return (builtin_exit(cmd, (*env), minishell));
	else if (cmd->args[0] && ft_strncmp(cmd->args[0], "cd", 2) == 0 && cmd->args[0][2] == '\0')
		return (builtin_cd(cmd->args));
	else if (cmd->args[0] && ft_strncmp(cmd->args[0], "env", 3) == 0 && cmd->args[0][3] == '\0')
		return (builtin_env((*env)));
	else if (cmd->args[0] && ft_strncmp(cmd->args[0], "export", 6) == 0 && cmd->args[0][6] == '\0')
		return (builtin_export(cmd->args, env));
	else if (cmd->args[0] && ft_strncmp(cmd->args[0], "unset", 5) == 0 && cmd->args[0][5] == '\0')
		return (builtin_unset(cmd->args[1], env));
	else if (cmd->args[0] && ft_strncmp(cmd->args[0], "pwd", 3) == 0 && cmd->args[0][3] == '\0')
		return (builtin_pwd());
	else if (cmd->args[0] && ft_strncmp(cmd->args[0], "echo", 4) == 0 && cmd->args[0][4] == '\0')
		return (builtin_echo(cmd->args));
	return (1);
}

int	is_builtin(char **cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd[0], "cd")
		|| !ft_strcmp(cmd[0], "echo")
		|| !ft_strcmp(cmd[0], "exit")
		|| !ft_strcmp(cmd[0], "pwd")
		|| !ft_strcmp(cmd[0], "env")
		|| !ft_strcmp(cmd[0], "export")
		|| !ft_strcmp(cmd[0], "unset"));
}
