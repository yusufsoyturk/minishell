#include "../inc/minishell.h"

static int	builtin_cd(char **args, t_shell *mini)
{
	char	*cwd;
	char	*newpwd;
	char	*oldpwd;
	char	*target;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	oldpwd = getenv("OLDPWD");
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		target = getenv("HOME");
		if (!target)
		{
			free(cwd);
			return (1);
		}
		if (chdir(target) == -1)
		{
			perror("cd");
			free(cwd);
			mini->last_status = 1;
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (!oldpwd || ft_strcmp(oldpwd, cwd) == 0)
		{
			if (chdir("..") == -1)
			{
				perror("cd");
				free(cwd);
				mini->last_status = 1;
				return (1);
			}
			else
			{
				target = getcwd(NULL, 0);
				printf("%s\n", target);
				free(target);
			}
		}
		else
		{
			if (chdir(oldpwd) == -1)
			{
				perror("cd");
				free(cwd);
				mini->last_status = 1;
				return (1);
			}
			else
			{
				printf("%s\n", oldpwd);
			}
		}
		setenv("OLDPWD", cwd, 1);
		newpwd = getcwd(NULL, 0);
		if (newpwd)
		{
			setenv("PWD", newpwd, 1);
			free(newpwd);
		}
		free(cwd);
		mini->last_status = 0;
		return (0);
	}
	else
	{
		target = args[1];
		if (chdir(target) == -1)
		{
			perror("cd");
			free(cwd);
			mini->last_status = 1;
			return (1);
		}
	}
	setenv("OLDPWD", cwd, 1);

	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		setenv("PWD", newpwd, 1);
		free(newpwd);
	}
	free(cwd);
	mini->last_status = 0;
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
	write(1, "exit\n", 5);
	if (cmd->args[1])
	{
		if ((ft_strncmp(cmd->args[1], "-", 1) || ft_strncmp(cmd->args[1], "+", 1)))
			i++;
		while (cmd->args[1][++i])
		{
			if (ft_isdigit(cmd->args[1][i]) == 0)
			{
				exit_error(cmd->args[1], "numeric argument required", "exit");
				minishell->last_status = 2;
				return (1);
			}
		}
		if (cmd->args[2])
		{
			ft_putendl_fd("cmd: exit: too many arguments", 2);
			minishell->last_status = 1;
			return (1);
		}
		exit_code = ft_atoll(cmd->args[1]) % 256;
	}
	free_max(minishell, env_list, cmd);
	exit(exit_code);
}

int	built(t_command *cmd, t_env **env, t_shell *minishell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd, (*env), minishell));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		if (cmd->args[1] && cmd->args[2])
		{
			ft_putendl_fd("minishell: cd: too many arguments", 2);
			minishell->last_status = 1;
			return (-1);
		}
		return (builtin_cd(cmd->args, minishell));
	}
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env((*env)));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd->args, env, minishell));
	if (ft_strcmp(cmd->args[0], "unset") == 0 && cmd->args[1])
		return (builtin_unset(cmd->args[1], env));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->args[0], "echo") == 0)
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
