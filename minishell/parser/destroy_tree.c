/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:51:34 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/29 14:41:41 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	destroy_p(t_node_pipe *node);
void	destroy_r(t_node_redirect *node);
void	destroy_e(t_node_execution *node);

void	destroy_tree(t_node_default *node)
{
	if (node)
	{
		if (node->node_type & 1 << 6)
			destroy_p((t_node_pipe *)node);
		else if (node->node_type & 1 << 5)
			destroy_r((t_node_redirect *)node);
		else if (node->node_type & 1 << 4)
			destroy_e((t_node_execution *)node);
	}
}

void	destroy_p(t_node_pipe *node)
{
	destroy_tree(node->left_node);
	destroy_tree(node->right_node);
	if (node)
		free(node);
}

void	destroy_r(t_node_redirect *node)
{
	destroy_tree(node->next);
	if (node->filename)
		free(node->filename);
	if (node)
		free(node);
}

void	destroy_e(t_node_execution *node)
{
	int	i;

	i = 0;
	while (i < node->n_params)
	{
		if (node->params[i])
			free(node->params[i]);
		i++;
	}
	if (node->command)
		free(node->command);
	if (node->params)
		free(node->params);
	if (node)
		free(node);
}
