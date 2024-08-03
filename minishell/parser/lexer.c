/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:46:47 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:14:05 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>

static char	*next_token(char *str)
{
	while (1)
	{
		str = until_charset(str, "|<>$\"'", 1, 0);
		if (*str == '\'')
		{
			str = until_charset(++str, "'", 0, 0);
			if (*str)
				str++;
		}
		else if (*str == '"')
		{
			str = until_charset(++str, "\"", 0, 0);
			if (*str)
				str++;
		}
		else if (*str == '$')
		{
			if (*++str != '?')
				str = until_charset(str, "<>|", 1, 1);
			else
				str++;
		}
		else
			return (str);
	}
}

static t_token	lex_init(t_lex lex)
{
	*lex.cpos = *lex.str;
	return ((t_token){});
}

t_token	lex(char *str, int *wstatus)
{
	static char	*cpos;
	static int	status;	
	char		*end;
	char		*content;

	if (wstatus)
		status = *wstatus;
	if (str)
		return (lex_init((t_lex){&str, &cpos, &status, &end, &content}));
	cpos = skip_space(cpos);
	if (*cpos == '\n' || *cpos == '\0')
		return ((t_token){EOL, NULL});
	end = next_token(cpos);
	if (end != cpos)
		return (lex_cmd((t_lex){&str, &cpos, &status, &end, &content}));
	if (*cpos == '<' || *cpos == '>')
		return (lex_redir((t_lex){&str, &cpos, &status, &end, &content}));
	if (*cpos == '|')
		return (lex_pipe((t_lex){&str, &cpos, &status, &end, &content}));
	return ((t_token){EOL, NULL});
}
