/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:12:15 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 15:59:43 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../minishell.h"
# include "libft.h"

# define EOL 1
# define ERR 0

typedef struct s_redirect_headtail
{
	t_node_r	*head;
	t_node_r	*tail;
}	t_redirect_tailhead;

typedef struct s_lex_context
{
	char	**str;
	char	**cpos;
	int		*status;
	char	**end;
	char	**content;
}	t_lex;

typedef struct s_pipe_context
{
	t_node_p	*new_node;
	t_node_d	*branch;
	t_token		token;
}	t_pipe;

typedef struct s_redir_context
{
	t_node_r	**tail;
	t_node_r	**new_node;
	t_token		*token;
	t_node_d	**branch;
	t_node_d	**root;
}	t_redir;

/* ************************************************************************** */
// lexer_utils.c
int			ft_isspace(int c);
char		*skip_space(char *str);
char		*until_charset(char *str, char *charset, int space, int oalnum);
char		*ft_strndup(const char *s, size_t n);

/* ************************************************************************** */
// lexer_redir.c
t_token		lex_hd(t_lex lex);
t_token		lex_in(t_lex lex);
t_token		lex_app(t_lex lex);
t_token		lex_out(t_lex lex);
t_token		lex_redir(t_lex lex);

/* ************************************************************************** */
// lexer_cmd.c
t_token		lex_cmd(t_lex lex);

/* ************************************************************************** */
// lexer_pipe.c
t_token		lex_pipe(t_lex lex);

/* ************************************************************************** */
// lexer.c
char		*next_token(char *str);
t_token		lex(char *str, int *wstatus);

/* ************************************************************************** */
// expand.c
char		*expand(char *str, int status);

void		parse_p(t_node_d **root);
t_token		parse_r(t_node_d **branch, t_node_d **root, t_node_e *node_exec);
t_token		parse_e(t_node_d **branch, t_node_d **root);

int			check_tree(t_node_d *node);

t_node_d	*print_tree(t_node_d *root);

#endif
