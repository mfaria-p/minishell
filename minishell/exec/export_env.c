/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:09:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/02 16:56:06 by ecorona-         ###   ########.fr       */
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

void	set_env_with_equal_plus(char ***envp, char *var_value)
{
	int		index;
	char	*key;
	char	*new_value;

	// Duplicate var_value to manipulate it
	key = strdup(var_value);
	if (key == NULL)
		return ;
	char	*plus_equal_sign = ft_strchr(key, '+');
	*plus_equal_sign = '\0'; // Terminate key to get the correct key
	index = find_var(*envp, key);
	if (index >= 0)
	{
		char	*equal_sign = ft_strchr((*envp)[index], '=');
		if (equal_sign)
		{
			// Append the new value to the existing value
			new_value = malloc(strlen((*envp)[index]) + strlen(plus_equal_sign + 2) + 1);
			if (new_value == NULL)
			{
				free(key);
				return ;
			}
			strcpy(new_value, (*envp)[index]);
			strcat(new_value, plus_equal_sign + 2);
			free((*envp)[index]);
			(*envp)[index] = new_value;
		}
		else
		{
			new_value = malloc(strlen((*envp)[index]) + strlen(plus_equal_sign + 2) + 2);
			if (new_value == NULL)
			{
				free(key);
				return ;
			}
			strcpy(new_value, (*envp)[index]);
			strcat(new_value, "=");
			strcat(new_value, plus_equal_sign + 2);
			free((*envp)[index]);
			(*envp)[index] = new_value;
		}
	}
	else
	{
		*plus_equal_sign = '+';
		char	*equal_sign = ft_strchr(var_value, '=');
		size_t	key_len = plus_equal_sign - key;
	    size_t	value_len = strlen(equal_sign + 1);
		new_value = malloc(key_len + value_len + 2);
		if (new_value == NULL)
		{
			free(key);
			return ;
		}
		strncpy(new_value, var_value, plus_equal_sign - key); // Copy part before `+=`
		new_value[plus_equal_sign - key] = '='; // Replace `+` with `=`
		strcpy(new_value + (plus_equal_sign - key) + 1, equal_sign + 1); // Copy part after `=`
		if (resize_and_add(envp, new_value) == NULL)
		{
			free(key);
			free(new_value);
			return ;
		}
		*plus_equal_sign = '+';
		free(new_value); // Free allocated memory
	}
	free(key);
}

void	ft_doexport(t_env *env, char **params, int *wstatus)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (params[i] != NULL)
	{
		equal_sign = ft_strchr(params[i], '=');
		if (equal_sign != NULL && *(equal_sign - 1) != '+')
		{
			*equal_sign = '\0';
			*wstatus = !is_valid_identifier(params[i], equal_sign + 1);
			if (!*wstatus)
			{
				*equal_sign = '=';
				set_env_with_equal(&env->export, params[i]);
				set_env_with_equal(&env->envp, params[i]);
			}
		}
		else if (equal_sign == NULL)
		{
			*wstatus = !is_valid_identifier(params[i], NULL);
			if (!*wstatus)
				set_env_without_equal(&env->export, params[i]);
		}
		else
		{
			*equal_sign = '\0';
			*(equal_sign - 1) = '\0';
			*wstatus = !is_valid_identifier_plus(params[i], equal_sign + 1);
			if (!*wstatus)
			{
				*equal_sign = '=';
				*(equal_sign - 1) = '+';
				set_env_with_equal_plus(&env->export, params[i]);
				set_env_with_equal_plus(&env->envp, params[i]);
			}
		}
		i++;
	}
}
