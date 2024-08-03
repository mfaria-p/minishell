/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:46:47 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 15:20:49 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>

char	*next_token(char *str)
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

t_token	lex_cmd(t_lex lex)
{
	*lex.content = ft_strndup(*lex.cpos, *lex.end - *lex.cpos + 1);
	*lex.content = expand(*lex.content, *lex.status);
	if (!*lex.content)
		return ((t_token){ERR, NULL, **lex.cpos});
	*lex.cpos = *lex.end;
	return ((t_token){E_cmd, *lex.content});
}

t_token	lex_hd(t_lex lex)
{
	(*lex.cpos)++;
	*lex.cpos = skip_space(*lex.cpos);
	*lex.end = next_token(*lex.cpos);
	if (*lex.end == *lex.cpos)
	{
		write(STDERR_FILENO, "minishell: no redirect file\n", 28);
		return ((t_token){ERR, NULL, **lex.cpos});
	}
	*lex.content = ft_strndup(*lex.cpos, *lex.end - *lex.cpos + 1);
	*lex.content = expand(*lex.content, *lex.status);
	if (!*lex.content)
		return ((t_token){ERR, NULL, **lex.cpos});
	*lex.cpos = *lex.end;
	return ((t_token){R_heredoc, *lex.content});
}

t_token	lex_in(t_lex lex)
{
	*lex.cpos = skip_space(*lex.cpos);
	*lex.end = next_token(*lex.cpos);
	if (*lex.end == *lex.cpos)
	{
		write(STDERR_FILENO, "minishell: no redirect file\n", 28);
		return ((t_token){ERR, NULL, **lex.cpos});
	}
	*lex.content = ft_strndup(*lex.cpos, *lex.end - *lex.cpos + 1);
	*lex.content = expand(*lex.content, *lex.status);
	if (!*lex.content)
		return ((t_token){ERR, NULL, **lex.cpos});
	*lex.cpos = *lex.end;
	return ((t_token){R_input, *lex.content});
}

t_token	lex_app(t_lex lex)
{
	(*lex.cpos)++;
	*lex.cpos = skip_space(*lex.cpos);
	*lex.end = next_token(*lex.cpos);
	if (*lex.end == *lex.cpos)
	{
		write(STDERR_FILENO, "minishell: no heredoc delimeter\n", 33);
		return ((t_token){ERR, NULL, **lex.cpos});
	}
	*lex.content = ft_strndup(*lex.cpos, *lex.end - *lex.cpos + 1);
	*lex.content = expand(*lex.content, *lex.status);
	if (!*lex.content)
		return ((t_token){ERR, NULL, **lex.cpos});
	*lex.cpos = *lex.end;
	return ((t_token){R_app, *lex.content});
}

t_token	lex_out(t_lex lex)
{
	*lex.cpos = skip_space(*lex.cpos);
	*lex.end = next_token(*lex.cpos);
	if (*lex.end == *lex.cpos)
	{
		write(STDERR_FILENO, "minishell: no redirect file\n", 28);
		return ((t_token){ERR, NULL, **lex.cpos});
	}
	*lex.content = ft_strndup(*lex.cpos, *lex.end - *lex.cpos + 1);
	*lex.content = expand(*lex.content, *lex.status);
	if (!*lex.content)
		return ((t_token){ERR, NULL, **lex.cpos});
	*lex.cpos = *lex.end;
	return ((t_token){R_out, *lex.content});
}

t_token	lex_pipe(t_lex lex)
{
	(*lex.cpos)++;
	return ((t_token){P, NULL});
}

t_token	lex_redir(t_lex lex)
{
	if (**lex.cpos == '<')
	{
		(*lex.cpos)++;
		if (**lex.cpos == '<')
			return (lex_hd(lex));
		return (lex_in(lex));
	}
	if (**lex.cpos == '>')
	{
		(*lex.cpos)++;
		if (**lex.cpos == '>')
			return (lex_app(lex));
		return (lex_out(lex));
	}
}

t_token	lex_init(t_lex lex)
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
