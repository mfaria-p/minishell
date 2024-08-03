/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:53:16 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:41:22 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_d	*parse(void)
{
	t_node_d	*root;

	root = NULL;
	parse_p(&root);
	if (check_tree(root) < 0)
	{
		destroy_tree(root);
		root = NULL;
	}
	return (root);
}

void	parse_p(t_node_d **root)
{
	static t_node_p	*tail;
	t_pipe			pipe;

	pipe.branch = NULL;
	pipe.token = parse_e(&pipe.branch, root);
	if (pipe.token.code & 1 << 6)
	{
		pipe.new_node = ft_calloc(1, sizeof(t_node_p));
		pipe.new_node->type = pipe.token.code;
		pipe.new_node->lnode = (t_node_d *)pipe.branch;
		if (tail)
			tail->rnode = (t_node_d *)pipe.new_node;
		tail = pipe.new_node;
		if (*root == NULL || !((*root)->type & 1 << 6))
			*root = (t_node_d *)pipe.new_node;
		parse_p(root);
	}
	else if (tail && pipe.token.code == EOL)
		tail->rnode = (t_node_d *)pipe.branch;
	else if (pipe.token.code == ERR)
	{
		destroy_tree(*root);
		*root = NULL;
	}
	tail = NULL;
}

t_token	parse_e(t_node_d **branch, t_node_d **root)
{
	static t_node_e	*exec;
	char			**new_params;
	t_token			token;

	token = parse_r(branch, root, exec);
	while (token.code & 1 << 4)
	{
		if (exec == NULL)
			parse_cmd(&exec, &token, branch);
		else
			parse_param(&exec, &new_params, &token);
		if (*root == NULL)
			*root = (t_node_d *)exec;
		token = parse_r(branch, root, exec);
	}
	exec = NULL;
	return (token);
}

t_token	parse_r(t_node_d **branch, t_node_d **root, t_node_e *node_exec)
{
	static t_node_r	*tail;
	t_node_r		*new_node;
	t_token			token;

	token = lex(NULL, NULL);
	while (token.code & 1 << 5)
	{
		parse_r_loop((t_redir){&tail, &new_node, &token, branch, root});
		tail->next = (t_node_d *)node_exec;
		token = lex(NULL, NULL);
	}
	return (token);
}
