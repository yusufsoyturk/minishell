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
    t_command *tmp;
    int i;

    while (cmd)
    {
        tmp = cmd;
        cmd = cmd->next;
        if (tmp->args)
        {
            i = 0;
            while (tmp->args[i])
            {
                free(tmp->args[i]);
                i++;
            }
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

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void	free_max(t_shell *minishell, t_env *env, t_command *cmd)
{
	free_env(env);
	free_double(minishell);
	free_token(minishell->token);
	free(minishell->line);
	free(minishell);
	free_commands(cmd);
}

void	free_less(t_shell *minishell, t_command *commands)
{
	free_double(minishell);
	free_token(minishell->token);
	free(minishell->line);
	free_commands(commands);
}
