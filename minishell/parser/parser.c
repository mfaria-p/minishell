/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:53:16 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 15:46:11 by ecorona-         ###   ########.fr       */
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

void	parse_cmd(t_node_e **exec, t_token *token, t_node_d **branch)
{
	*exec = ft_calloc(1, sizeof(t_node_e));
	(*exec)->type = token->code;
	(*exec)->n_params = 0;
	(*exec)->command = token->content;
	if (*branch == NULL)
		*branch = (t_node_d *)*exec;
}

void	parse_param(t_node_e **exec, char ***new_params, t_token *token)
{
	int	i;

	*new_params = ft_calloc((*exec)->n_params + 2, sizeof(char *));
	(*exec)->n_params++;
	i = 0;
	while ((*exec)->params && (*exec)->params[i])
	{
		(*new_params)[i] = (*exec)->params[i];
		i++;
	}
	(*new_params)[i++] = token->content;
	(*new_params)[i] = NULL;
	if ((*exec)->params)
		free((*exec)->params);
	(*exec)->params = *new_params;
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

void	parse_r_loop(t_redir redir)
{
	const char		*temp_file_name = "/tmp/heredoc_tmp";

	if (redir.token->code == R_heredoc)
		create_heredoc(redir.token->content, temp_file_name);
	*redir.new_node = ft_calloc(1, sizeof(t_node_r));
	(*redir.new_node)->type = redir.token->code;
	(*redir.new_node)->filename = redir.token->content;
	(*redir.new_node)->delimeter = redir.token->content;
	if (*redir.branch == NULL || (*redir.branch)->type & 1 << 4)
	{
		*redir.branch = (t_node_d *)(*redir.new_node);
		*redir.tail = NULL;
	}
	if (*redir.root == NULL || (*redir.root)->type & 1 << 4)
		*redir.root = (t_node_d *)(*redir.new_node);
	if (*redir.tail)
	{
		(*redir.new_node)->next = (*redir.tail)->next;
		(*redir.tail)->next = (t_node_d *)(*redir.new_node);
		*redir.tail = *redir.new_node;
	}
	else
		*redir.tail = *redir.new_node;
}

t_token	parse_r(t_node_d **branch, t_node_d **root, t_node_e *node_exec)
{
	static t_node_r	*tail;
	const char		*temp_file_name = "/tmp/heredoc_tmp";
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
