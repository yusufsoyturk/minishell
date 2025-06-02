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

int	token_check(char *str)
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
		return (1);
	}
	free(prefix);
	return (0);
}

t_command	*pars(t_token *token, t_env *env)
{
	t_command	*head;
	t_command	*current;

	head = init_command();
	current = head;
	(void)env;
	while (token)
	{
		if (token->type == T_PIPE)
		{
			current->next = init_command();
			current = current->next;
			token = token->next;
			if (!token)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
				return (NULL);
			}
			token_check(token->value);
		}
		else if (token->type != T_PIPE && token->type != T_WORD && token->type != T_ENV_VAR)
		{//BURASI KOMPLE BAŞKA BİR FONKSİYONA ALINABİLİR
			current->redirs = malloc(sizeof(t_redir));
			if (token->type == T_REDIR_IN)
                current->redirs->flag = O_RDONLY;
            else if (token->type == T_REDIR_OUT)
                current->redirs->flag = O_CREAT | O_WRONLY | O_TRUNC;
            else if (token->type == T_REDIR_APPEND)
                current->redirs->flag = O_CREAT | O_WRONLY | O_APPEND;
			else if (token->type == T_REDIR_HEREDOC)
				current->redirs->flag = R_HEREDOC;
			current->redirs->fd = -1;
			token = token->next; //token'da ilerlememizin sebebi redirectionların targete her zaman sağ tarafta olur
			if (!token)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
				return (NULL);
			}
			token_check(token->value);
			current->redirs->target = ft_strdup(token->value);//sıkıntıyı anladım ama zamanım yok
			current->redirs->next = NULL;
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
