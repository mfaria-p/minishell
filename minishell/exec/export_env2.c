/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:13:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/18 00:45:33 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// Function to resize the environment array and add a new variable
char	**resize_and_add_envp(char ***envp, char *new_var, int *j)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while ((*envp)[count] != NULL)
		count++;
	new_envp = malloc((count + 2) * sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		i++;
	}
	if (*j == 1)
		ft_free(*envp);
	else
		*j == 1;
	new_envp[i] = ft_strdup(new_var);
	new_envp[i + 1] = NULL;
	*envp = new_envp;
	return (new_envp);
}

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
