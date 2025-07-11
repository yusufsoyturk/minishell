/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktoraman <ktoraman@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:58:59 by ktoraman          #+#    #+#             */
/*   Updated: 2025/07/11 14:35:17 by ktoraman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int execute(t_command *cmd, t_env **env_list, t_shell *mini)
{
	t_command	*current;
	pid_t		pid;
	int			pipe_fd[2];
	int			prev_fd;
	int			status;
	int			loop;

	loop = 0;
	prev_fd = -1;
	status = 0;
	current = cmd;
	while (current)
	{
		mini->last_status = 0;
		if (handle_redirection(current, (*env_list), mini) < 0)
		{
			mini->last_status = 1;
			if (current->next)
			{
				if (pipe(pipe_fd) < 0)
					perror("pipe");
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
				current = current->next;
				mini->last_status = 0;
				continue ;
			}
			return (mini->last_status);
		}
			if (!current->redirs && !current->next && is_builtin(current->args) && !loop)
				return (built(current, env_list, mini));
			if (current->next && pipe(pipe_fd) < 0)
			return (perror("pipe"), 1);
			pid = fork();
			if (pid < 0)
				return (perror("fork"), 1);
			ignore_signals();
			if (pid == 0)
			{
				setup_child_signals();
				if (!current->args)
				{
					free_max(mini, *env_list, cmd);
					exit(0);
				}
				if (current->input != STDIN_FILENO)
					dup2(current->input, STDIN_FILENO);
				else if (prev_fd != -1)
					dup2(prev_fd, STDIN_FILENO);
				if (current->output != STDOUT_FILENO)
					dup2(current->output, STDOUT_FILENO);
				else if (current->next)
					dup2(pipe_fd[1], STDOUT_FILENO);
				if (current->next)
				{
					close(pipe_fd[0]);
					close(pipe_fd[1]);
				}
				if (is_builtin(current->args))
				{
					built(current, env_list, mini);
					free_struct(mini);
					free_env_list((*env_list));
					free_commands(cmd);
					exit(0);
				}
				if (!current->args[0] || current->args[0][0] == '\0')
				{
					ft_putstr_fd("minishell: command not found\n", 2);
					free_env_list((*env_list));
					free_struct(mini);
					free_commands(cmd);
					exit(127);
				}
				char **char_env;
				char_env = env_to_envp_array(*env_list);
				char *exec_path = get_path(current->args[0], char_env);
				if (!exec_path)
				{
					if (access(current->args[0], F_OK) == 0)
						exec_path = ft_strdup(current->args[0]);
					else	
					{
						ft_putstr_fd("minishell: ", 2);
						ft_putstr_fd(current->args[0], 2);
						ft_putendl_fd(": command not found", 2);
						free_env_list((*env_list));
						free_struct(mini);
						ft_free_tab(char_env);
						free_commands(cmd);
						exit(127);
					}
				}
				free_env_list((*env_list));
				free_struct(mini);
				check_permissions_exec(exec_path, cmd, char_env);
				execve(exec_path, current->args, char_env);
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->args[0], 2);
				ft_putendl_fd(": command not found", 2);
				free_commands(cmd);
				exit(127);
			}
			else
			{
				if (prev_fd != -1)
					close(prev_fd);
				if (current->next)
				{
					close(pipe_fd[1]);
					prev_fd = pipe_fd[0];
				}
				loop = 1;
			}
			current = current->next;
		}
	while (waitpid(-1, &status, 0) > 0)
		;
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
		write(1, "\n", 1);
	}
	setup_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
		mini->last_status = 0;
	else if (WIFEXITED(status))
		mini->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->last_status = 128 + WTERMSIG(status);
	else 
		mini->last_status = 1;
	return (mini->last_status);
}
