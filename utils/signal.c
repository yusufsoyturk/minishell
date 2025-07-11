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
    struct sigaction	sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	ignore_signals(void)
{
	struct	sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

void heredoc_sigint_handler(int signo)
{
	(void)signo;
	close(STDIN_FILENO);
	g_sigint_received = 1;
}

void setup_heredoc_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}