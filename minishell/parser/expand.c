/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 17:57:40 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/24 19:14:47 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*ft_strnadd(char const *s1, char const *s2, size_t n)
{
	char	*join;
	size_t	len_s1;
	size_t	len_s2;

	if (n == 0 || !s2 || !*s2)
		return ((char *)s1);
	if (!s1)
		return (ft_strndup(s2, n));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	join = ft_calloc(len_s1 + len_s2 + 1, sizeof(char));
	if (!join)
		return (0);
	ft_strlcat(join, s1, len_s1 + 1);
	if (n < len_s2)
		len_s2 = n;
	ft_strlcat(join, s2, len_s1 + len_s2 + 1);
	if (s1)
		free((void *)s1);
	return (join);
}

char	*ft_stradd(char const *s1, char const *s2)
{
	char	*join;
	size_t	len_s1;
	size_t	len_s2;

	if (!s2 || !*s2)
		return ((char *)s1);
	if (!s1)
		return (ft_strdup(s2));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	join = ft_calloc(len_s1 + len_s2 + 1, sizeof(char));
	if (!join)
		return (0);
	ft_strlcat(join, s1, len_s1 + 1);
	ft_strlcat(join, s2, len_s1 + len_s2 + 1);
	if (s1)
		free((void *)s1);
	return (join);
}

char	*ft_getenv(char *str, size_t n)
{
	char	*envvar_name;
	char	*result;

	envvar_name = ft_strndup(str, n);
	result = getenv(envvar_name);
	free(envvar_name);
	return (result);
}

char	*expand(char *str)
{
	char	*result;
	char	*start;
	char	*end;

	result = NULL;
	start = str;
	while (*start)
	{
		end = until_charset(start, "\"'$", 0, 0);
		result = ft_strnadd(result, start, end - start + 1);
		start = end;
		if (*end == '\'')
		{
			start = ++end;
			end = until_charset(start, "'", 0, 0);
			result = ft_strnadd(result, start, end - start);
			start = ++end;
			continue ;
		}
		if (*end == '"')
		{
			start = ++end;
			end = until_charset(start, "\"$", 0, 0);
			result = ft_strnadd(result, start, end - start);
			while (*end == '$')
			{
				start = ++end;
				end = until_charset(start, "\"$", 1, 1);
				result = ft_stradd(result, ft_getenv(start, end - start + 1));
				start = end;
				end = until_charset(start, "\"$", 0, 0);
				result = ft_strnadd(result, start, end - start);
			}
			start = ++end;
			continue ;
		}
		if (*end == '$')
		{
			start = ++end;
			end = until_charset(start, NULL, 1, 1);
			result = ft_stradd(result, ft_getenv(start, end - start + 1));
			start = end;
			continue ;
		}
	}
	free(str);
	return (result);
}
