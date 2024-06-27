/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 21:18:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/27 22:30:53 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void    init(char **export, char **envp)
{
    
}

void	ft_printexport(char **export)
{
	int	i;

	i = 0;
	while (export[i] != NULL)
	{
		printf("declare -x %s\n", export[i]);
		i++;
	}
}

int	is_valid_identifier(char *var, char *value)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[0]) && var[0] != '_')
	{
		error_identifier(var, value);
		return (0);
	}
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
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

void	sort_env(char **envp)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	while (envp[i] != NULL)
	{
		j = i + 1;
		while (envp[j] != NULL)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
}

// Function to find the variable in the environment
int	find_var(char **envp, const char *var)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, len) == 0 && (envp[i][len] == '='))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}