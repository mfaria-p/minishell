/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:46:47 by ecorona-          #+#    #+#             */
/*   Updated: 2024/07/21 09:59:33 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*next_token(char *str)
{
	while (1)
	{
		str = until_charset(str, "|<>$\"'", 1, 0);
		if (*str == '\'')
		{
			str++;
			str = until_charset(str, "'", 0, 0);
			str++;
		}
		else if (*str == '"')
		{
			str++;
			str = until_charset(str, "\"", 0, 0);
			str++;
		}
		else if (*str == '$')
		{
			str++;
			if (*str != '?')
				str = until_charset(str, "=<>|", 1, 1);
			else
				str++;
		}
		else
			return (str);
	}
}

t_token	lex(char *str)
{
	static char	*cpos;
	static int	state;	
	char		*end;
	char		*content;

	if (str)
	{
		cpos = str;
		return ((t_token){});
	}
	cpos = skip_space(cpos);
	if (*cpos == '\n' || *cpos == '\0')
		return ((t_token){EOL, NULL});
	end = next_token(cpos);
	if (end != cpos)
	{
		content = ft_strndup(cpos, end - cpos + 1);
		content = expand(content);
		cpos = end;
		return ((t_token){E_cmd, content});
	}
	if (*cpos == '<')
	{
		cpos++;
		if (*cpos == '<')
		{
			cpos++;
			cpos = skip_space(cpos);
			end = until_charset(cpos, "<>|", 1, 0);
			content = ft_strndup(cpos, end - cpos + 1);
			content = expand(content);
			cpos = end;
			return ((t_token){R_heredoc, content});
		}
		else
		{
			cpos = skip_space(cpos);
			end = until_charset(cpos, "<>|", 1, 0);
			content = ft_strndup(cpos, end - cpos + 1);
			content = expand(content);
			cpos = end;
			return ((t_token){R_input, content});
		}
	}
	if (*cpos == '>')
	{
		cpos++;
		if (*cpos == '>')
		{
			cpos++;
			cpos = skip_space(cpos);
			end = until_charset(cpos, "<>|", 1, 0);
			content = ft_strndup(cpos, end - cpos + 1);
			content = expand(content);
			cpos = end;
			return ((t_token){R_app, content});
		}
		else
		{
			cpos = skip_space(cpos);
			end = until_charset(cpos, "<>|", 1, 0);
			content = ft_strndup(cpos, end - cpos + 1);
			content = expand(content);
			cpos = end;
			return ((t_token){R_out, content});
		}
	}
	if (*cpos == '|')
	{
		cpos++;
		return ((t_token){P, NULL});
	}
	return ((t_token){EOL, NULL});
}
