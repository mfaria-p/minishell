/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 17:57:40 by ecorona-          #+#    #+#             */
/*   Updated: 2024/07/31 17:05:01 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <unistd.h>

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

char	*ft_getenv(char *str, size_t n)
{
	char	*envvar_name;
	char	*result;

	envvar_name = ft_strndup(str, n);
	result = getenv(envvar_name);
	free(envvar_name);
	return (result);
}

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

int	check_dquote(char **start, char **end, char **result, int status)
{
	char *exit_status;

	*start = ++(*end);
	*end = until_charset(*start, "\"$", 0, 0);
	*result = ft_strnadd(*result, *start, *end - *start);
	while (**end == '$')
	{
		if (ft_isspace(*(*end + 1)) || ft_strchr("\n\"=", *(*end + 1)) || !*(*end + 1) || ft_isdigit(*(*end + 1)))
			*result = ft_strnadd(*result, "$", 1);
		if (*(*end + 1) == '?')
		{
			(*end)++;
			(*start) = ++(*end);
			exit_status = ft_itoa(status);
			*result = ft_stradd(*result, exit_status);
			free(exit_status);
			*end = until_charset(*start, "\"$", 0, 0);
			continue ;
		}
		*start = ++(*end);
		*end = until_charset(*start, "\"$", 1, 1);
		*result = ft_stradd(*result, ft_getenv(*start, *end - *start + 1));
		*start = *end;
		*end = until_charset(*start, "\"$", 0, 0);
		*result = ft_strnadd(*result, *start, *end - *start);
	}
	if (**end == '\0')
	{
		write(STDERR_FILENO, "minishell: unclosed dquote\n", 28);
		return (-1);
	}
	else
		*start = ++(*end);
	return (0);
}

void	check_envvar(char **start, char **end, char **result)
{
	if (ft_isspace(*(*end + 1)) || ft_strchr("\n=", *(*end + 1)) || !*(*end + 1) || ft_isdigit(*(*end + 1)))
	{
		*result = ft_strnadd(*result, "$", 1);
		*start = ++(*end);
	}
	else
	{
		*start = ++(*end);
		*end = until_charset(*start, NULL, 1, 1);
		*result = ft_stradd(*result, ft_getenv(*start, *end - *start + 1));
		*start = *end;
	}
}

char	*expand(char *str, int status)
{
	char	*result;
	char	*start;
	char	*end;
	char	*exit_status;

	result = ft_calloc(1, sizeof(char));
	*result = '\0';
	start = str;
	while (*start)
	{
		end = until_charset(start, "\"'$", 0, 0);
		result = ft_strnadd(result, start, end - start);
		start = end;
		if (*end == '\'')
		{
			if (check_squote(&start, &end, &result) < 0)
			{
				free(result);
				free(str);
				return (NULL);
			}
			continue ;
		}
		if (*end == '$')
		{
			if (*(end + 1) == '?')
			{
				end++;
				start = ++end;
				exit_status = ft_itoa(status);
				result = ft_stradd(result, exit_status);
				free(exit_status);
				continue ;
			}
			else if (*(end + 1) == '"')
				start = ++end;
			else if (ft_isspace(*(end + 1)) || *(end + 1) == '\n' || !*(end + 1))
			{
				result = ft_strnadd(result, "$", 1);
				start = ++end;
			}
			else
			{
				check_envvar(&start, &end, &result);
				continue ;
			}
		}
		if (*end == '"')
		{
			if (check_dquote(&start, &end, &result, status) < 0)
			{
				free(result);
				free(str);
				return (NULL);
			}
			continue ;
		}
	}
	free(str);
	return (result);
}
