/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:24:43 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/04 09:10:12 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_squote(char **start, char **end, char **result)
{
	*start = ++(*end);
	*end = until_charset(*start, "'", 0, 0);
	*result = ft_strnadd(*result, *start, *end - *start);
	if (**end == '\0')
	{
		write(STDERR_FILENO, "minishell: unclosed squote\n", 28);
		return (-1);
	}
	else
		*start = ++(*end);
	return (0);
}

void	check_envvar(char **start, char **end, char **result)
{
	if (ft_isspace(*(*end + 1)) || !ft_validenv(*(*end + 1)) \
		|| !*(*end + 1) || ft_isdigit(*(*end + 1)))
	{
		*result = ft_strnadd(*result, "$", 1);
		*start = ++(*end);
	}
	else
	{
		*start = ++(*end);
		*end = until_charset(*start, NULL, 1, 1);
		*result = ft_stradd(*result, ft_getenv(0, *start, *end - *start + 1));
		*start = *end;
	}
}

int	check_dsign(char **start, char **end, char **result, int status)
{
	if (*(*end + 1) == '?')
	{
		(*end)++;
		(*start) = ++(*end);
		*result = add_status(*result, status);
		return (1);
	}
	else if (*(*end + 1) == '"')
		*start = ++(*end);
	else if (ft_isspace(*(*end + 1)) || *(*end + 1) == '\n' || !*(*end + 1))
	{
		*result = ft_strnadd(*result, "$", 1);
		*start = ++(*end);
	}
	else
	{
		check_envvar(start, end, result);
		return (1);
	}
	return (0);
}
