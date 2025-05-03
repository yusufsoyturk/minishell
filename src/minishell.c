#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    char	*input;
    char    **args;
    (void)argc;
    (void)env;
    (void)argv;
    // input = ft_strdup(argv[1]);
    while (1)
    {
        input = readline("minishell> ");
        args = ft_split(input, ' ');
        built(args, input);
        // pars(input, env);
        //execute
        ft_free_tab(args);
        free(input);
    }
    return (0);
}