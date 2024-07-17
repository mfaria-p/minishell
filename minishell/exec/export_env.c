/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:09:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/18 00:35:40 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
	int		var_len;
	int		value_len;
	char	*new_value;

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

void	set_env_with_equal_envp(char ***envp, char *var_value, int *i)
{
	int	index;
	int	var_len;
	int	value_len;

	index = find_var(*envp, var_value);
	if (index >= 0)
	{
		if (*i == 1)
			free((*envp)[index]);
		else
			*i == 1;
		(*envp)[index] = ft_strdup(var_value);
	}
	else
	{
		if (resize_and_add_envp(envp, var_value, i) == NULL)
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

void	ft_doexport(t_env *env, char **params)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (params[i] != NULL)
	{
		equal_sign = ft_strchr(params[i], '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			if (is_valid_identifier(params[i], equal_sign + 1) == 1)
			{
				*equal_sign = '=';
				set_env_with_equal(&env->export, params[i]);
				// a=bº da um enter a mais
				set_env_with_equal_envp(&env->envp, params[i], &env->i);
			}
		}
		else
		{
			if (is_valid_identifier(params[i], NULL))
				set_env_without_equal(&env->export, params[i]);
		}
		i++;
	}
}
