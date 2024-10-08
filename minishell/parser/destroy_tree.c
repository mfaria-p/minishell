/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:51:34 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/05 11:11:55 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	destroy_p(t_node_p *node)
{
	destroy_tree(node->lnode);
	destroy_tree(node->rnode);
	if (node)
		free(node);
	node = NULL;
}

static void	destroy_r(t_node_r *node)
{
	destroy_tree(node->next);
	if (node->filename)
		free(node->filename);
	node->filename = NULL;
	if (node)
		free(node);
	node = NULL;
}

static void	destroy_e(t_node_e *node)
{
	int	i;

	i = 0;
	while (i < node->n_params)
	{
		if (node->params[i])
			free(node->params[i]);
		node->params[i] = NULL;
		i++;
	}
	if (node->command)
		free(node->command);
	node->command = NULL;
	if (node->params)
		free(node->params);
	node->params = NULL;
	if (node)
		free(node);
	node = NULL;
}

void	destroy_tree(t_node_d *node)
{
	if (node)
	{
		if (node->type & 1 << 6)
			destroy_p((t_node_p *)node);
		else if (node->type & 1 << 5)
			destroy_r((t_node_r *)node);
		else if (node->type & 1 << 4)
			destroy_e((t_node_e *)node);
	}
}
