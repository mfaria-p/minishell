/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:23:04 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/05 09:18:52 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>

void	exec_pipe(t_node_p *pip, t_sh sh)
{
	int		pipefd[2];
	pid_t	pid[2];

	if (pipe(pipefd) == -1)
		ft_error(2);
	pid[0] = have_child(pip, PIPE_WRITE, pipefd, sh);
	pid[1] = have_child(pip, PIPE_READ, pipefd, sh);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], sh.stat, 0);
	child_signal(*sh.stat);
	*sh.stat = WEXITSTATUS(*sh.stat);
}

int	is_regular_file(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISREG(path_stat.st_mode));
}
