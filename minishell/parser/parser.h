/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:12:15 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/29 12:37:43 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "../minishell.h"
# include "libft.h"

# define EOL 0

/*
typedef struct s_token
{
	int		code;
	char	*content;
}	t_token;

//Nodes structures
// node_type := PIPE, REDIRECT, EXEC
typedef struct s_node_default
{
	int	node_type;
}	t_node_default;

typedef struct s_node_pipe
{
	int					node_type;
	t_node_default		*left_node;
	t_node_default		*right_node;
}	t_node_pipe;

typedef struct s_node_redirect
{
	int					node_type;
	char				*filename;
	char				*delimeter;
	t_node_default		*next;
}	t_node_redirect;

typedef struct s_node_execution
{
	int		node_type;
	char	*command;
	int		n_params;
	char	**params;
}	t_node_execution;

//Node types
enum e_nodetype
{
	E_cmd = 0b1<<4,
	E_builtin,
	R_out = 0b1<<5,
	R_app,
	R_heredoc,
	R_input,
	P = 0b1<<6
};
*/

typedef struct s_redirect_headtail
{
	t_node_redirect	*head;
	t_node_redirect	*tail;
}	t_redirect_tailhead;

int		ft_isspace(int c);
char	*skip_space(char *str);
char	*until_charset(char *str, char *charset, int until_space, int oalnum);
char	*ft_strndup(const char *s, size_t n);
char	*expand(char *str);

char	*next_token(char *str);
t_token	lex(char *str);

// void			parse_e(t_token token, t_node_execution **node_execution, int *flags);
// void			parse_r(t_token token, t_redirect_tailhead *node_redirect, int *flags);
// void			parse_p(t_token token, t_node_pipe **node_pipe, int *flags);
// t_node_default	*parse(char	*str);

void	parse_p(t_node_default **root);
t_token	parse_r(t_node_default **branch, t_node_default **root, t_node_execution *node_exec);
t_token	parse_e(t_node_default **branch, t_node_default **root);
// t_node_default	*parse(void);

void	print_tree(t_node_default *root);
#endif
