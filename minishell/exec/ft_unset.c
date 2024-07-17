/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:13:55 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/18 00:37:11 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**remove_var(char **envp, char *var)
{
	int		i;
	int		j;
	char	**new_envp;

	i = 0;
	j = 0;
	new_envp = allocate_env_array(envp);
	while (envp[i] != NULL)
	{
		if (matches_var(envp[i], var, ft_strlen(var)))
		{
			i++;
			continue ;
		}
		new_envp[j] = strdup(envp[i]);
		j++;
		i++;
	}
	new_envp[j] = NULL;
	ft_free(envp);
	return (new_envp);
}

int	matches_var(char *env_entry, char *var, int len2)
{
	char *equal_sign;
    int len1;
	
	equal_sign = strchr(env_entry, '=');
	if (equal_sign != NULL)
	{
		*equal_sign = '\0';
		len1 = strlen(env_entry);
	}
	else
		len1 = strlen(env_entry);
	if (len1 == len2 && strncmp(env_entry, var, len2 + 1) == 0)
	{
		if (equal_sign != NULL)
			*equal_sign = '=';
		return (1);
	}
	if (equal_sign != NULL)
		*equal_sign = '=';
	return (0);
}

char	**allocate_env_array(char **envp)
{
	char	**new_envp;
	int		count;

	count = 0;
	while (envp[count] != NULL)
		count++;
	new_envp = (char **)malloc(count * sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	return (new_envp);
}

// Main function to remove the variable from the environment
char	**remove_var_envp(char **envp, char *var, int *k)
{
	int		i;
	int		j;
	char	**new_envp;

	i = 0;
	j = 0;
	new_envp = allocate_env_array(envp);
	while (envp[i] != NULL)
	{
		if (matches_var(envp[i], var, ft_strlen(var)))
		{
			i++;
			continue ;
		}
		new_envp[j] = strdup(envp[i]);
		j++;
		i++;
	}
	new_envp[j] = NULL;
	if (*k == 1)
		ft_free(envp);
	else
		*k == 1;
	return (new_envp);
}

void	ft_unset(char **args, t_env *env)
{
	int	i;
	int	j;

	i = 0;
	if (!args || !*args)
		return ;
	while (args[i])
	{
		if (find_var(env->envp, args[i]) != -1)
		{
			env->envp = remove_var_envp(env->envp, args[i], &env->i);
			env->export = remove_var(env->export, args[i]);
		}
		else if (find_var(env->export, args[i]) != -1)
			env->export = remove_var(env->export, args[i]);
		i++;
	}
}
