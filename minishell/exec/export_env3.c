/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 21:18:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/25 23:07:23 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_printenv(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
}

int	valid_id(char *var, char *value)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(var[0]) || var[0] == '_'))
	{
		error_identifier(var, value);
		return (0);
	}
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
		{
			error_identifier(var, value);
			return (0);
		}
		i++;
	}
	if (!value)
		return (2);
	return (1);
}

int	valid_id_plus(char *var, char *value)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(var[0]) || var[0] == '_'))
	{
		error_identifier_plus(var, value);
		return (0);
	}
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
		{
			error_identifier_plus(var, value);
			return (0);
		}
		i++;
	}
	if (!value)
		return (2);
	return (1);
}

int	get_var_length(const char *var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign != NULL)
	{
		return (equal_sign - var);
	}
	return (ft_strlen(var));
}

int	compare_env_var(const char *env_var, const char *var, int len2)
{
	int	len1;

	len1 = get_var_length(env_var);
	if (len1 == len2 && ft_strncmp(env_var, var, len1) == 0)
	{
		return (1);
	}
	return (0);
}

int	find_var(char **envp, const char *var)
{
	int	len2;
	int	i;

	i = 0;
	len2 = get_var_length(var);
	while (envp[i] != NULL)
	{
		if (compare_env_var(envp[i], var, len2))
			return (i);
		i++;
	}
	return (-1);
}
