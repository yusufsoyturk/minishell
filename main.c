#include "inc/minishell.h"

void    write_args(char **args)
{
    int i;
    i = 0;
    while (args[i])
    {
        printf("%s\n", args[i]);
        i++;
    }
}

static void ft_init_shell(t_shell *minishell)
{
    ft_memset(minishell, 0, sizeof(t_shell));
}

int main(int ac, char **av, char **env)
{
    t_shell *minishell;
    minishell = malloc(sizeof(t_shell));
    ft_init_shell(minishell);
    (void)env;
    while (ac == 1 && av[0])
    {
        minishell->line = readline("minishell>");
        printf("%s\n", new_word(minishell->line));
        // printf("%s\n", minishell->line);
        minishell->args = ft_split(minishell->line, ' ');
        built(minishell->args, minishell->line);
        // pars(input, env);
        //execute
        free_double(minishell);
        free(minishell->line);
    }
    free_struct(minishell);
    return (0);
}
