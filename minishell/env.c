/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:59:14 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/05 09:54:39 by ecorona-         ###   ########.fr       */
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

char	*ft_getenv(t_env *env, char *str, size_t n)
{
	static t_env	*env2;
	char			*envvar_name;
	int				index;
	char			*value;

	if (env)
	{
		env2 = env;
		return (NULL);
	}
	envvar_name = ft_strndup(str, n);
	index = find_var(env2->envp, envvar_name);
	if (index < 0)
	{
		free(envvar_name);
		return (NULL);
	}
	value = ft_strchr((env2->envp)[index], '=');
	free(envvar_name);
	return (value + 1);
}
