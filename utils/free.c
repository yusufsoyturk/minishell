#include "../inc/minishell.h"

void	free_double(t_shell *minishell)
{
	int	i;

	i = 0;
	while (minishell->args[i])
	{
		free(minishell->args[i]);
		i++;
	}
	free(minishell->args);
}

void	free_struct(t_shell *minishell)
{
    if (minishell->line)
	    free(minishell->line);
	if (minishell->args)
        free_double(minishell);
	if (minishell->token)
        free_token(minishell->token);
    if (minishell)
	    free(minishell);
}

void	free_env_list(t_env *env_list)
{
    t_env *tmp;

    while (env_list)
    {
        tmp = env_list;
        env_list = env_list->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

void free_redirections(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}


void free_commands(t_command *cmd)
{
    t_command	*tmp;
    int			i;

    while (cmd)
    {
        tmp = cmd;
        cmd = cmd->next;
        if (tmp->args)
        {
            for (i = 0; tmp->args[i]; i++)
                free(tmp->args[i]);
            free(tmp->args);
        }
        free_redirections(tmp->redirs);
        free(tmp);
    }
}

void	free_env(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
