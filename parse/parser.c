#include "../inc/minishell.h"
#include "../inc/lexer.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	cmd->args = NULL;
	cmd->input = STDIN_FILENO;
	cmd->output = STDOUT_FILENO;
	cmd->redirs = NULL;
	cmd->next = NULL;

	return (cmd);
}

int add_arg_to_command(t_command *cmd, const char *arg)
{
    int 	count;
	int		i;
	char	**new_args;

	i = -1;
	count = 0;
    while (cmd->args && cmd->args[count])
        count++;

    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return (-1);

    while (++i < count)
        new_args[i] = cmd->args[i];

    new_args[count] = ft_strdup(arg);
    new_args[count + 1] = NULL;

    free(cmd->args);
    cmd->args = new_args;

    return (0);
}
int	token_check_pipe(char *str, t_shell *mini)
{
	char	*prefix;
	char	*msg;
	char	*tmp;

	prefix = ft_strdup("minishell: syntax error near unexpected token `");
	if (ft_strncmp(str, "&", 1) == 0 || ft_strncmp(str, "|", 1) == 0
				|| ft_strncmp(str, "&&", 2) == 0 || ft_strncmp(str, "||", 2) == 0)
	{
		tmp = ft_strjoin(prefix, str);
		free(prefix);
		msg = ft_strjoin(tmp, "'");
		free(tmp);
		ft_putendl_fd(msg, 2);
		free(msg);
		mini->last_status = 2;
		return (1);
	}
	free(prefix);
	return (0);
}

int	token_check(char *str, t_shell *mini)
{
	char	*prefix;
	char	*msg;
	char	*tmp;

	prefix = ft_strdup("minishell: syntax error near unexpected token `");
	if (ft_strncmp(str, "<", 1) == 0 || ft_strncmp(str, ">", 1) == 0
		|| ft_strncmp(str, "<<", 1) == 0 || ft_strncmp(str, ">>", 1) == 0
			|| ft_strncmp(str, "&", 1) == 0 || ft_strncmp(str, "|", 1) == 0
				|| ft_strncmp(str, "&&", 2) == 0 || ft_strncmp(str, "||", 2) == 0)
	{
		tmp = ft_strjoin(prefix, str);
		free(prefix);
		msg = ft_strjoin(tmp, "'");
		free(tmp);
		ft_putendl_fd(msg, 2);
		free(msg);
		mini->last_status = 2;
		return (1);
	}
	free(prefix);
	return (0);
}

t_command	*pars(t_token *token, t_env *env, t_shell *mini)
{
	t_command	*head;
	t_command	*current;

	head = init_command();
	current = head;
	(void)env;
	if (!token)
		return (NULL);
	if (token->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (free_commands(head), NULL);
	}
	while (token)
	{
		if (token->type == T_PIPE)
		{
			current->next = init_command();
			current = current->next;
			token = token->next;
			if (!token)
			{
				mini->last_status = 2;
				ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
				return (free_commands(head), NULL);
			}
			if (token_check_pipe(token->value, mini))
				return (free_commands(head), NULL);
		}
		else if (token->type != T_PIPE && token->type != T_WORD && token->type != T_ENV_VAR)
		{
			t_redir *new_redir = malloc(sizeof(t_redir));
			if (!new_redir)
				return (free_commands(head), NULL);
			new_redir->next = NULL;
			new_redir->here_flag = 0;
			new_redir->fd = -1;
			if (token->type == T_REDIR_IN)
				new_redir->flag = O_RDONLY;
			else if (token->type == T_REDIR_OUT)
				new_redir->flag = O_CREAT | O_WRONLY | O_TRUNC;
			else if (token->type == T_REDIR_APPEND)
				new_redir->flag = O_CREAT | O_WRONLY | O_APPEND;
			else if (token->type == T_REDIR_HEREDOC)
			{
				if (token->here_flag == 1)
					new_redir->here_flag = 1;
				new_redir->flag = R_HEREDOC;
			}
			token = token->next;
			if (!token || token_check(token->value, mini))
			{
				free(new_redir);
				mini->last_status = 2;
				ft_putendl_fd("minishell: syntax error near unexpected token", 2);
				return (free_commands(head), NULL);
			}
			new_redir->target = ft_strdup(token->value);
			if (!current->redirs)
				current->redirs = new_redir;
			else
			{
				t_redir *tmp = current->redirs;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = new_redir;
			}
			token = token->next;
		}
		else
		{
			add_arg_to_command(current, token->value);
			token = token->next;
		}
	}
	return (head);
}
