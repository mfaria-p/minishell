/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 19:47:10 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/24 22:32:49 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

t_node_default	*parse(char	*str)
{
	t_token						token;
	static t_node_pipe			*node_pipe;
	static t_redirect_tailhead	node_redirect;
	static t_node_execution		*node_execution;
	static int					tonew;

	tonew = 0b111;
	token = lex(str);
	while (token.code != EOL)
	{
		if (token.code & 0b1 << 4)
		{
			parse_e(token, &node_execution, &tonew);
			if (!(tonew & 0b010))
				node_redirect.tail->next = (t_node_default *)node_execution;
			if (node_pipe && !node_pipe->right_node)
				node_pipe->right_node = (t_node_default *)node_execution;
		}
		else if (token.code & 0b1 << 5)
		{
			parse_r(token, &node_redirect, &tonew);
			if (node_pipe)
				node_pipe->right_node = (t_node_default *)node_redirect.head;
			if (!(tonew & 0b001))
				node_redirect.tail->next = (t_node_default *)node_execution;
		}
		else if (token.code & 0b1 << 6)
		{
			parse_p(token, &node_pipe, &tonew);
			if (!node_pipe->left_node && node_redirect.head)
				node_pipe->left_node = (t_node_default *)node_redirect.head;
			else if (!node_pipe->left_node)
				node_pipe->left_node = (t_node_default *)node_execution;
		}
		token = lex(NULL);
	}
	if (node_pipe)
		return ((t_node_default *)node_pipe);
	else if (node_redirect.head)
		return ((t_node_default *)node_redirect.head);
	else if (node_execution)
		return ((t_node_default *)node_execution);
	else
		return (NULL);
}

void	parse_e(t_token token, t_node_execution **node_execution, int *flags)
{
	t_node_execution	*new_node;
	char				**new_params;
	int					i;

	new_node = *node_execution;
	if (*flags & 0b001)
	{
		new_node = ft_calloc(1, sizeof(t_node_execution));
		*node_execution = new_node;
		new_node->node_type = token.code; // WILL ALWAYS BE E_CMD AND NEVER E_BUILDIN
		new_node->n_params = 0;
	}
	*flags &= 0b110;
	if (!new_node->command)
		new_node->command = token.content;
	else
	{
		new_params = ft_calloc(new_node->n_params + 2, sizeof(char *));
		new_node->n_params++;
		i = 0;
		while (new_node->params && new_node->params[i])
		{
			new_params[i] = new_node->params[i];
			i++;
		}
		new_params[i++] = token.content;
		new_params[i] = NULL;
		if (new_node->params)
			free(new_node->params);
		new_node->params = new_params;
	}
}

void	parse_r(t_token token, t_redirect_tailhead *node_redirect, int *flags)
{
	t_node_redirect			*new_node;

	if (*flags & 0b010)
	{
		new_node = ft_calloc(1, sizeof(t_node_redirect));
		node_redirect->head = new_node;
		node_redirect->tail = new_node;
		new_node->node_type = token.code;
		new_node->filename = token.content;
		new_node->delimeter = token.content;
	}
	else
	{
		new_node = ft_calloc(1, sizeof(t_node_redirect));
		new_node->next = node_redirect->tail->next;
		node_redirect->tail->next = (t_node_default *)new_node;
		node_redirect->tail = new_node;
		new_node->node_type = token.code;
		new_node->filename = token.content;
		new_node->delimeter = token.content;
	}
	*flags &= 0b101;
}

void	parse_p(t_token token, t_node_pipe **node_pipe, int *flags)
{
	t_node_pipe	*new_node;

	new_node = ft_calloc(1, sizeof(t_node_pipe));
	new_node->left_node = (t_node_default *)*node_pipe;
	*node_pipe = new_node;
	new_node->node_type = token.code;
	*flags |= 0b011;
	*flags &= 0b011;
}
