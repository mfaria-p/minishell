/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:59:14 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/03 16:59:36 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	init_env(char ***export, char ***envp2, char **envp)
{
	t_env	env;
	int		count;
	int		i;

	count = 0;
	while (envp[count] != NULL)
		count++;
	*export = malloc((count + 1) * sizeof(char *));
	*envp2 = malloc((count + 1) * sizeof(char *));
	if (!*export || !*envp2)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		(*export)[i] = ft_strdup(envp[i]);
		(*envp2)[i] = ft_strdup(envp[i]);
		i++;
	}
	(*export)[count] = NULL;
	(*envp2)[count] = NULL;
	env.export = *export;
	env.envp = *envp2;
	return (env);
}

// Function to free the export array in the environment structure
void	free_env_export(t_env *env)
{
	int	i;

	i = 0;
	if (env->export)
	{
		while (env->export[i] != NULL)
		{
			free(env->export[i]);
			i++;
		}
		free(env->export);
	}
	i = 0;
	if (env->envp)
	{
		while (env->envp[i] != NULL)
		{
			free(env->envp[i]);
			i++;
		}
		free(env->envp);
	}
}
