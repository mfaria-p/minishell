/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:01:07 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 17:01:22 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fd_init(t_fds *fd)
{
	fd->in = dup(STDIN_FILENO);
	fd->out = dup(STDOUT_FILENO);
}

void	fd_close(t_fds *fd)
{
	close(fd->in);
	close(fd->out);
}
