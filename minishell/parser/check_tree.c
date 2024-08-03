/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:51:34 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:19:29 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	check_p(t_node_p *node, int *status);
static void	check_r(t_node_r *node, int *status);

static void	check_tree_aux(t_node_d *node, int *status)
{
	if (node)
	{
		if (node->type & 1 << 6)
			check_p((t_node_p *)node, status);
		else if (node->type & 1 << 5)
			check_r((t_node_r *)node, status);
	}
}

static void	check_p(t_node_p *node, int *status)
{
	if (!node->lnode || !node->rnode)
		*status = -1;
	check_tree_aux(node->lnode, status);
	check_tree_aux(node->rnode, status);
}

static void	check_r(t_node_r *node, int *status)
{
	check_tree_aux(node->next, status);
}

static void	check_tree_err(int status)
{
	if (status == -1)
		write(STDERR_FILENO, "minishell: incomplete pipe\n", 28);
}

int	check_tree(t_node_d *node)
{
	int	status;

	status = 0;
	if (node)
	{
		if (node->type & 1 << 6)
			check_p((t_node_p *)node, &status);
		else if (node->type & 1 << 5)
			check_r((t_node_r *)node, &status);
	}
	check_tree_err(status);
	return (status);
}
