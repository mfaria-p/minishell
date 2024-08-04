/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_check_dquote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 09:09:23 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/04 09:10:10 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	check_dquote_help(char **start, char **end, char **res, int expand)
{
	*start = ++(*end);
	*end = until_charset(*start, "\"$", 1, 1);
	if (expand)
		*res = ft_stradd(*res, ft_getenv(NULL, *start, *end - *start + 1));
	else
		*res = ft_strnadd(*res, *start, *end - *start);
	*start = *end;
	*end = until_charset(*start, "\"$", 0, 0);
	*res = ft_strnadd(*res, *start, *end - *start);
}

static void	check_envvar_dquote(char **start, char **end, char **result, int st)
{
	if (ft_isspace(*(*end + 1)) || !ft_validenv(*(*end + 1)) \
		|| !*(*end + 1) || ft_isdigit(*(*end + 1)))
	{
		*result = ft_strnadd(*result, "$", 1);
		check_dquote_help(start, end, result, 0);
	}
	else if (*(*end + 1) == '?')
	{
		(*end)++;
		*result = add_status(*result, st);
		check_dquote_help(start, end, result, 0);
	}
	else
		check_dquote_help(start, end, result, 1);
}

int	check_dquote(char **start, char **end, char **result, int status)
{
	*start = ++(*end);
	*end = until_charset(*start, "\"$", 0, 0);
	*result = ft_strnadd(*result, *start, *end - *start);
	while (**end == '$')
		check_envvar_dquote(start, end, result, status);
	if (**end == '\0')
	{
		write(STDERR_FILENO, "minishell: unclosed dquote\n", 28);
		return (-1);
	}
	*start = ++(*end);
	return (0);
}
