/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 21:16:37 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/24 22:21:56 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void	write_node(int daddy, int	*n_node, t_node_default *node);

void	print_tree(t_node_default *root)
{
	int		fd;
	int		n_node = 0;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		fd = open("tree.mermaid", O_CREAT | O_RDWR | O_TRUNC, 00777);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		printf("graph TD\n");
		write_node(n_node, &n_node, root);
		exit(1);
	}
	else
		wait(&pid);
}

void	write_p(int daddy, int	*n_node, t_node_pipe *node_pipe)
{
	int	this;

	this = *n_node + 1;
	if (daddy)
		printf("\t%i-->%i\n", daddy, this);
	*n_node = this;
	printf("\t%i((PIPE))\n", this);
	write_node(this, n_node, node_pipe->left_node);
	write_node(this, n_node, node_pipe->right_node);
}

void	write_r(int daddy, int *n_node, t_node_redirect *node_redirect)
{
	int	this;

	this = *n_node + 1;
	if (daddy)
		printf("\t%i-->%i\n", daddy, this);
	*n_node = this;
	printf("\t%i((", this);
	switch (node_redirect->node_type)
	{
		case R_app: printf(">> ");break;
		case R_out: printf("> ");break;
		case R_heredoc: printf("<< ");break;
		case R_input: printf("< ");break;
	}
	printf("%s))\n", node_redirect->filename);
	write_node(this, n_node, node_redirect->next);
}

void	write_e(int daddy, int	*n_node, t_node_execution *node_execution)
{
	int	this;

	this = *n_node + 1;
	if (daddy)
		printf("\t%i-->%i\n", daddy, this);
	*n_node = this;
	printf("\t%i((%s))\n", this, node_execution->command);
}

void	write_node(int daddy, int	*n_node, t_node_default *node)
{
	if (node->node_type & 0b1 << 4)
		write_e(daddy, n_node, (t_node_execution *)node);
	else if (node->node_type & 0b1 << 5)
		write_r(daddy, n_node, (t_node_redirect *)node);
	else if (node->node_type & 0b1 << 6)
		write_p(daddy, n_node, (t_node_pipe *)node);
}
