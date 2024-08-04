/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 17:57:40 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:25:33 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	*expand_free(char *str, char *result)
{
	free(result);
	free(str);
	return (NULL);
}

int	expand_loop(char **start, char **end, char **result, int status)
{
	while (**start)
	{
		*end = until_charset(*start, "\"'$", 0, 0);
		*result = ft_strnadd(*result, *start, *end - *start);
		*start = *end;
		if (**end == '\'')
		{
			if (check_squote(start, end, result) < 0)
				return (-1);
			continue ;
		}
		if (**end == '$')
			if (check_dsign(start, end, result, status))
				continue ;
		if (**end == '"')
		{
			if (check_dquote(start, end, result, status) < 0)
				return (-1);
			continue ;
		}
	}
	return (0);
}

char	*expand(char *str, int status)
{
	char	*result;
	char	*start;
	char	*end;

	result = ft_calloc(1, sizeof(char));
	*result = '\0';
	start = str;
	if (expand_loop(&start, &end, &result, status) < 0)
		return (expand_free(str, result));
	free(str);
	return (result);
}
