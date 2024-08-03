/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:53:16 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 12:53:35 by ecorona-         ###   ########.fr       */
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
	t_node_p		*new_node;
	t_node_d		*branch;
	t_token			token;

	branch = NULL;
	token = parse_e(&branch, root);
	if (token.code & 1 << 6)
	{
		new_node = ft_calloc(1, sizeof(t_node_p));
		new_node->type = token.code;
		new_node->lnode = (t_node_d *)branch;
		if (tail)
			tail->rnode = (t_node_d *)new_node;
		tail = new_node;
		if (*root == NULL || !((*root)->type & 1 << 6))
			*root = (t_node_d *)new_node;
		parse_p(root);
	}
	else if (tail && token.code == EOL)
		tail->rnode = (t_node_d *)branch;
	else if (token.code == ERR)
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
	int				i;

	token = parse_r(branch, root, exec);
	while (token.code & 1 << 4)
	{
		if (exec == NULL)
		{
			exec = ft_calloc(1, sizeof(t_node_e));
			exec->type = token.code;
			exec->n_params = 0;
			exec->command = token.content;
			if (*branch == NULL)
				*branch = (t_node_d *)exec;
		}
		else
		{
			new_params = ft_calloc(exec->n_params + 2, sizeof(char *));
			exec->n_params++;
			i = 0;
			while (exec->params && exec->params[i])
			{
				new_params[i] = exec->params[i];
				i++;
			}
			new_params[i++] = token.content;
			new_params[i] = NULL;
			if (exec->params)
				free(exec->params);
			exec->params = new_params;
		}
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
	const char		*temp_file_name = "/tmp/heredoc_tmp";

	token = lex(NULL, NULL);
	while (token.code & 1 << 5)
	{
		if (token.code == R_heredoc)
			create_heredoc(token.content, temp_file_name);
		new_node = ft_calloc(1, sizeof(t_node_r));
		new_node->type = token.code;
		new_node->filename = token.content;
		new_node->delimeter = token.content;
		if (*branch == NULL || (*branch)->type & 1 << 4)
		{
			*branch = (t_node_d *)new_node;
			tail = NULL;
		}
		if (*root == NULL || (*root)->type & 1 << 4)
			*root = (t_node_d *)new_node;
		if (tail)
		{
			new_node->next = tail->next;
			tail->next = (t_node_d *)new_node;
			tail = new_node;
		}
		else
			tail = new_node;
		tail->next = (t_node_d *)node_exec;
		token = lex(NULL, NULL);
	}
	return (token);
}
