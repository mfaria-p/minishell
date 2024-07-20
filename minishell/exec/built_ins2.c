/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:13:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/14 20:30:06 by ecorona-         ###   ########.fr       */
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
	new_envp = malloc((count + 2) * sizeof(char *)); //+2 for var and NULL
	if (new_envp == NULL)
	{
		free(new_var);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	/* if (*j == 1)
		ft_free(envp);
	else
		*j == 1; */
	new_envp[i] = new_var;
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
	new_envp = malloc((count + 2) * sizeof(char *)); //+2 for var and NULL
	if (new_envp == NULL)
	{
		free(new_var);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	/* ft_free(*envp); */
	free(*envp);
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	*envp = new_envp;
	return (new_envp);
}

/* void	ft_free(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
} */

// Function to set the environment variable WHEN VALUE EXISTS E ELES METEM IGUAL E O VALOR
void	set_env_with_equal(char ***envp, char *var_value)
{
	int	index;
	int	var_len;
	int	value_len;
	char	*new_value;

	index = find_var(*envp, var_value);
	if (index >= 0)
	{
		new_value = ft_strdup(var_value);
		free((*envp)[index]);
		(*envp)[index] = new_value;
	}
	else
	{
		if (resize_and_add(envp, var_value) == NULL)
			return ; // Error already handled in resize_and_add
		//sort_env(*envp);
	}
}

void	set_env_with_equal_envp(char ***envp, char *var_value, int *i)
{
	int	index;
	int	var_len;
	int	value_len;
	char	*new_value;

	index = find_var(*envp, var_value);
	if (index >= 0)
	{
		new_value = ft_strdup(var_value);
		(*envp)[index] = new_value;
	}
	else
	{
		if (resize_and_add_envp(envp, var_value, i) == NULL)
			return ; // Error already handled in resize_and_add
		//sort_env(*envp);
	}
}

// Function to set the environment variable when there is no value
void	set_env_without_equal(char ***envp, char *var)
{
	int	index;

	index = find_var(*envp, var);
	if (index >= 0)
		return ; // Variable already exists, do nothing
	else
	{
		if (resize_and_add(envp, var) == NULL)
			return ; // Error already handled in resize_and_add
		//sort_env(*envp);
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
				//a=bº da um enter a mais
				// s tiver valor ent vai sermodificada em ambos
				set_env_with_equal_envp(&env->envp, params[i], &env->i);
			}
		}
		else
		{
			if (is_valid_identifier(params[i], NULL))
				set_env_without_equal(&env->export, params[i]);
			// se a var n tiver valor ent so é mostrada no export
		}
		i++;
	}
}
