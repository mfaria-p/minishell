/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:22:21 by ecorona-         ###   ########.fr       */
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
	if (n < len_s2)
		len_s2 = n;
	join = ft_calloc(len_s1 + len_s2 + 1, sizeof(char));
	if (!join)
		return (0);
	ft_strlcat(join, s1, len_s1 + 1);
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

char	*add_status(char *result, int status)
{
	char	*exit_status;

	exit_status = ft_itoa(status);
	result = ft_stradd(result, exit_status);
	free(exit_status);
	return (result);
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
