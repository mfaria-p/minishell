/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:53:16 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/29 12:53:42 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_default	*parse(void)
{
	t_node_default	*root;

	root = NULL;
	parse_p(&root);
	return (root);
}

void	parse_p(t_node_default **root)
{
	static t_node_pipe	*tail;
	t_node_pipe			*new_node;
	t_node_default		*branch;
	t_token				token;

	branch = NULL;
	token = parse_e(&branch, root);
	if (token.code & 1 << 6)
	{
		new_node = ft_calloc(1, sizeof(t_node_pipe));
		new_node->node_type = token.code;
		new_node->left_node = (t_node_default *)branch;
		if (tail)
			tail->right_node = (t_node_default *)new_node;
		tail = new_node;
		if (*root == NULL || !((*root)->node_type & 1 << 6))
			*root = (t_node_default *)new_node;
		parse_p(root);
	}
	else if (tail && token.code == EOL)
		tail->right_node = (t_node_default *)branch;
}

t_token	parse_e(t_node_default **branch, t_node_default **root)
{
	static t_node_execution	*exec;
	char					**new_params;
	t_token					token;
	int						i;

	token = parse_r(branch, root, exec);
	while (token.code & 1 << 4)
	{
		if (exec == NULL)
		{
			exec = ft_calloc(1, sizeof(t_node_execution));
			exec->node_type = token.code; // WILL ALWAYS BE E_CMD AND NEVER E_BUILDIN
			exec->n_params = 0;
			exec->command = token.content;
			if (*branch == NULL)
				*branch = (t_node_default *)exec;
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
			*root = (t_node_default *)exec;
		token = parse_r(branch, root, exec);
	}
	exec = NULL;
	return (token);
}

t_token	parse_r(t_node_default **branch, t_node_default **root, t_node_execution *node_exec)
{
	static t_node_redirect	*tail;
	t_node_redirect			*new_node;
	t_token					token;

	token = lex(NULL);
	while (token.code & 1 << 5)
	{
		new_node = ft_calloc(1, sizeof(t_node_redirect));
		new_node->node_type = token.code;
		new_node->filename = token.content;
		new_node->delimeter = token.content;
		if (*branch == NULL || (*branch)->node_type & 1 << 4)
		{
			*branch = (t_node_default *)new_node;
			tail = NULL;
		}
		if (*root == NULL || (*root)->node_type & 1 << 4)
			*root = (t_node_default *)new_node;
		if (tail)
		{
			new_node->next = tail->next;
			tail->next = (t_node_default *)new_node;
			tail = new_node;
		}
		else
			tail = new_node;
		tail->next = (t_node_default *)node_exec;
		token = lex(NULL);
	}
	return (token);
}
