/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:51:34 by ecorona-          #+#    #+#             */
/*   Updated: 2024/07/21 17:31:41 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>

int	check_p(t_node_pipe *node, int *status);
int	check_r(t_node_redirect *node, int *status);
int	check_e(t_node_execution *node, int *status);

void	check_tree_err(int status)
{
	if (status == -1)
		write(STDERR_FILENO, "minishell: incomplete pipe\n", 28);
}

int	check_tree(t_node_default *node)
{
	int	status;

	status = 0;
	if (node)
	{
		if (node->node_type & 1 << 6)
			check_p((t_node_pipe *)node, &status);
		else if (node->node_type & 1 << 5)
			check_r((t_node_redirect *)node, &status);
	}
	check_tree_err(status);
	return (status);
}

void	check_tree_aux(t_node_default *node, int *status)
{
	if (node)
	{
		if (node->node_type & 1 << 6)
			check_p((t_node_pipe *)node, status);
		else if (node->node_type & 1 << 5)
			check_r((t_node_redirect *)node, status);
	}
}

int	check_p(t_node_pipe *node, int *status)
{
	if (!node->left_node || !node->right_node)
		*status = -1;
	check_tree_aux(node->left_node, status);
	check_tree_aux(node->right_node, status);
}

int	check_r(t_node_redirect *node, int *status)
{
	check_tree_aux(node->next, status);
}
