/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:57:26 by ecorona-          #+#    #+#             */
/*   Updated: 2024/07/20 14:50:40 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <unistd.h>

extern int	g_sig;

static void	sighandler(int sig);

int	siginit(void)
{
	sigset_t			sa_mask;
	struct sigaction	sa;

	if (sigemptyset(&sa_mask) < 0 || sigaddset(&sa_mask, SIGINT) < 0 \
		|| sigaddset(&sa_mask, SIGQUIT) < 0 || sigaddset(&sa_mask, SIGUSR1) < 0)
	{
		printf("SIGSET INIT FAILED!\n");
		return (0);
	}
	sa.sa_mask = sa_mask;
	sa.sa_handler = sighandler;
	if (sigaction(SIGINT, &sa, NULL) < 0 || sigaction(SIGQUIT, &sa, NULL) < 0 \
		|| sigaction(SIGUSR1, &sa, NULL) < 0)
	{
		printf("SIGACTION FAILED!\n");
		return (0);
	}
	return (0);
}

void	sighandler(int sig)
{
	static char	c;
	static int	i;

	g_sig = sig;
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}
