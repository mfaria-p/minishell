/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:53:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:39:57 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
	return ((t_token){R_heredoc, *lex.content, '\0'});
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
	return ((t_token){R_input, *lex.content, '\0'});
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
	return ((t_token){R_app, *lex.content, '\0'});
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
	return ((t_token){R_out, *lex.content, '\0'});
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
	return ((t_token){});
}
