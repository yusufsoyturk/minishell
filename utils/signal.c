#include "../inc/minishell.h"

void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_sigint= 1;
}

void	setup_heredoc_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
