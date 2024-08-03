/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:57:26 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 17:38:31 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <signal.h>

static void	sighandler(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sig == SIGINT)
	{
		g_sig = sig;
		rl_replace_line("", 1);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	siginit(void)
{
	struct sigaction	act_int;
	struct sigaction	act_quit;

	g_sig = 0;
	ft_memset(&act_int, 0, sizeof(struct sigaction));
	ft_memset(&act_quit, 0, sizeof(struct sigaction));
	act_int.sa_sigaction = sighandler;
	act_int.sa_flags |= SA_SIGINFO;
	act_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_int, NULL) < 0 || \
		sigaction(SIGQUIT, &act_quit, NULL) < 0)
	{
		printf("SIGACTION FAILED!\n");
		return (0);
	}
	return (0);
}

void	sigignore(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);
}

void	sigchild(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &act, NULL);
}

void	child_signal(int wstatus)
{
	int		sig;
	int		nl;

	nl = 0;
	if (WIFSIGNALED(wstatus))
	{
		sig = WTERMSIG(wstatus);
		if (sig == SIGINT)
			nl = 1;
		else if (sig == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit", 4);
			nl = 1;
		}
		if (WCOREDUMP(wstatus))
			write(STDERR_FILENO, " (core dumped)", 14);
	}
	if (nl)
		write(STDERR_FILENO, "\n", 1);
}
