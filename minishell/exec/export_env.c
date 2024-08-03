/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:09:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/03 21:02:42 by ecorona-         ###   ########.fr       */
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

static void	set_env_index(char ***envp, t_setenv *setenv)
{
	if (setenv->equal)
	{
		setenv->new_val = malloc(strlen((*envp)[setenv->idx]) + strlen(setenv->plus + 2) + 1);
		if (setenv->new_val == NULL)
			free(setenv->key);
		else
		{
			strcpy(setenv->new_val, (*envp)[setenv->idx]);
			strcat(setenv->new_val, setenv->plus + 2);
			free((*envp)[setenv->idx]);
			(*envp)[setenv->idx] = setenv->new_val;
		}
		return ;
	}
	setenv->new_val = malloc(strlen((*envp)[setenv->idx]) + strlen(setenv->plus + 2) + 2);
	if (setenv->new_val == NULL)
		free(setenv->key);
	else
	{
		strcpy(setenv->new_val, (*envp)[setenv->idx]);
		strcat(setenv->new_val, "=");
		strcat(setenv->new_val, setenv->plus + 2);
		free((*envp)[setenv->idx]);
		(*envp)[setenv->idx] = setenv->new_val;
	}
}

static void	set_env_noindex(char ***envp, char *var_value, t_setenv *setenv)
{
	*setenv->plus = '+';
	setenv->equal = ft_strchr(var_value, '=');
	setenv->key_len = setenv->plus - setenv->key;
	setenv->val_len = strlen(setenv->equal + 1);
	setenv->new_val = malloc(setenv->key_len + setenv->val_len + 2);
	if (setenv->new_val == NULL)
		free(setenv->key);
	else
	{
		strncpy(setenv->new_val, var_value, setenv->plus - setenv->key);
		(setenv->new_val)[setenv->plus - setenv->key] = '=';
		strcpy(setenv->new_val + (setenv->plus - setenv->key) + 1, setenv->equal + 1);
		if (resize_and_add(envp, setenv->new_val) == NULL)
		{
			free(setenv->key);
			free(setenv->new_val);
		}
		else
		{
			*setenv->plus = '+';
			free(setenv->new_val);
		}
	}
}

void	set_env_with_equal_plus(char ***envp, char *var_value)
{
	t_setenv	setenv;

	setenv.key = strdup(var_value);
	if (setenv.key == NULL)
		return ;
	setenv.plus = ft_strchr(setenv.key, '+');
	*setenv.plus = '\0';
	setenv.idx = find_var(*envp, setenv.key);
	if (setenv.idx >= 0)
	{
		setenv.equal = ft_strchr((*envp)[setenv.idx], '=');
		set_env_index(envp, &setenv);
	}
	else
		set_env_noindex(envp, var_value, &setenv);
	free(setenv.key);
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
