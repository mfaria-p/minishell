/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 21:16:37 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/29 14:56:40 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void	write_node(int daddy, int	*n_node, t_node_d *node);

t_node_d	*print_tree(t_node_d *root)
{
	int		fd;
	int		n_node;
	pid_t	pid;

	n_node = 0;
	pid = fork();
	if (pid == 0)
	{
		fd = open("tree.mermaid", O_CREAT | O_RDWR | O_TRUNC, 00777);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		printf("graph TD\n");
		write_node(n_node, &n_node, root);
		destroy_tree(root);
		exit(1);
	}
	else
		wait(&pid);
	return (root);
}

void	write_p(int daddy, int	*n_node, t_node_p *node_pipe)
{
	int	this;

	this = *n_node + 1;
	if (daddy)
		printf("\t%i-->%i\n", daddy, this);
	*n_node = this;
	printf("\t%i((PIPE))\n", this);
	write_node(this, n_node, node_pipe->lnode);
	write_node(this, n_node, node_pipe->rnode);
}

void	write_r(int daddy, int *n_node, t_node_r *node_redirect)
{
	int	this;

	this = *n_node + 1;
	if (daddy)
		printf("\t%i-->%i\n", daddy, this);
	*n_node = this;
	printf("\t%i((\"", this);
	switch (node_redirect->type)
	{
		case R_app: printf(">> ");break;
		case R_out: printf("> ");break;
		case R_heredoc: printf("<< ");break;
		case R_input: printf("< ");break;
	}
	printf("%s\"))\n", node_redirect->filename);
	write_node(this, n_node, node_redirect->next);
}

void	write_e(int daddy, int	*n_node, t_node_e *node_execution)
{
	int	this;

	this = *n_node + 1;
	if (daddy)
		printf("\t%i-->%i\n", daddy, this);
	*n_node = this;
	printf("\t%i((\"%s\"))\n", this, node_execution->command);
}

void	write_node(int daddy, int	*n_node, t_node_d *node)
{
	if (node->type & 0b1 << 4)
		write_e(daddy, n_node, (t_node_e *)node);
	else if (node->type & 0b1 << 5)
		write_r(daddy, n_node, (t_node_r *)node);
	else if (node->type & 0b1 << 6)
		write_p(daddy, n_node, (t_node_p *)node);
}
