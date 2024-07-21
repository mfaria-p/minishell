/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:57:26 by ecorona-          #+#    #+#             */
/*   Updated: 2024/07/21 12:56:55 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	sighandler(int sig, siginfo_t *info, void *ucontext);
extern int	g_sig;

int	siginit(void)
{
	sigset_t			sa_mask;
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(struct sigaction));
	if (sigemptyset(&sa_mask) < 0 || sigaddset(&sa_mask, SIGINT) < 0 \
		|| sigaddset(&sa_mask, SIGQUIT) < 0)
	{
		printf("SIGSET INIT FAILED!\n");
		return (0);
	}
	act.sa_mask = sa_mask;
	act.sa_sigaction = sighandler;
	act.sa_flags |= SA_SIGINFO;
	if (sigaction(SIGINT, &act, NULL) < 0 || sigaction(SIGQUIT, &act, NULL) < 0)
	{
		printf("SIGACTION FAILED!\n");
		return (0);
	}
	return (0);
}

void	sighandler(int sig, siginfo_t *info, void *ucontext)
{
	static char	c;
	static int	i;

	(void)info;
	(void)ucontext;
	g_sig = sig;
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}
