/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:12:15 by ecorona-          #+#    #+#             */
/*   Updated: 2024/07/21 17:32:18 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "../minishell.h"
# include "libft.h"

# define EOL 1
# define ERR 0

typedef struct s_redirect_headtail
{
	t_node_redirect	*head;
	t_node_redirect	*tail;
}	t_redirect_tailhead;

int				ft_isspace(int c);
char			*skip_space(char *str);
char			*until_charset(char *str, char *charset, int until_space, int oalnum);
char			*ft_strndup(const char *s, size_t n);
char			*expand(char *str);

char			*next_token(char *str);
t_token			lex(char *str);

void			parse_p(t_node_default **root);
t_token			parse_r(t_node_default **branch, t_node_default **root, t_node_execution *node_exec);
t_token			parse_e(t_node_default **branch, t_node_default **root);

int	check_tree(t_node_default *node);

t_node_default	*print_tree(t_node_default *root);

#endif
