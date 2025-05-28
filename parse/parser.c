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
		}
		else if (token->type != T_PIPE && token->type != T_WORD)
		{//BURASI KOMPLE BAŞKA BİR FONKSİYONA ALINABİLİR
			t_redir	*r;
			r = malloc(sizeof(t_redir));
			if (token->type == T_REDIR_IN)
                r->flag = O_RDONLY;
            else if (token->type == T_REDIR_OUT)
                r->flag = O_CREAT | O_WRONLY | O_TRUNC;
            else if (token->type == T_REDIR_APPEND)
                r->flag = O_CREAT | O_WRONLY | O_APPEND;
			//heredoc problemli bir durum ona sonra bakacağım
			token = token->next; //token'da ilerlememizin sebebi redirectionların targete her zaman sağ tarafta olur
			r->target = ft_strdup(token->value);//sıkıntıyı anladım ama zamanım yok
			r->next = current->redirs;
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
