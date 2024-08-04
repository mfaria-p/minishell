/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 22:07:45 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/04 09:03:09 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	ft_isspace(int c)
{
	if (c == ' ')
		return (1);
	if (c == '\n')
		return (0);
	if (c >= '\t' && c <= '\r')
		return (1);
	return (0);
}

char	*skip_space(char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

int	ft_validenv(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*until_charset(char *str, char *charset, int until_space, int oalnum)
{
	while (*str && (!charset || !ft_strchr(charset, *str)) && \
		(!ft_isspace(*str) || !until_space) && ((ft_isalnum(*str) \
		|| *str == '_') || !oalnum))
		str++;
	return (str);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	size_s;
	char	*dup;

	size_s = ft_strlen(s) + 1;
	if (n < size_s)
		size_s = n;
	dup = ft_calloc(size_s, sizeof(char));
	if (!dup)
		return (0);
	ft_strlcpy(dup, s, size_s);
	return (dup);
}
