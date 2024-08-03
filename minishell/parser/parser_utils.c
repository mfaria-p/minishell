/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:06:40 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:07:16 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
