/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:13:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/04 00:32:09 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**resize_and_add(char ***envp, char *new_var)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while ((*envp)[count] != NULL)
		count++;
	new_envp = malloc((count + 2) * sizeof(char *));
	if (new_envp == NULL)
	{
		free(new_var);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		i++;
	}
	ft_free(*envp);
	new_envp[i] = ft_strdup(new_var);
	new_envp[i + 1] = NULL;
	*envp = new_envp;
	return (new_envp);
}

void	ft_printenv(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
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

void	set_env_with_equal(char ***envp, char *var_value)
{
	int		index;

	index = find_var(*envp, var_value);
	if (index >= 0)
	{
		free((*envp)[index]);
		(*envp)[index] = ft_strdup(var_value);
	}
	else
	{
		if (resize_and_add(envp, var_value) == NULL)
			return ;
	}
}

// Function to set the environment variable when there is no value
void	set_env_without_equal(char ***envp, char *var)
{
	int	index;

	index = find_var(*envp, var);
	if (index >= 0)
		return ;
	else
	{
		if (resize_and_add(envp, var) == NULL)
			return ;
	}
}
