/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:57:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:31:36 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	lex_cmd(t_lex lex)
{
	*lex.content = ft_strndup(*lex.cpos, *lex.end - *lex.cpos + 1);
	*lex.content = expand(*lex.content, *lex.status);
	if (!*lex.content)
		return ((t_token){ERR, NULL, **lex.cpos});
	*lex.cpos = *lex.end;
	return ((t_token){E_cmd, *lex.content});
}
