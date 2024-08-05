/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:09:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/05 10:20:19 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	set_env_index(char ***envp, t_setenv *setenv)
{
	if (setenv->equal)
	{
		setenv->new_val = env_alloc(envp, setenv, 1);
		if (setenv->new_val == NULL)
			free(setenv->key);
		else
		{
			ft_strlcpy(setenv->new_val, (*envp)[setenv->idx], -1);
			ft_strlcat(setenv->new_val, setenv->plus + 2, -1);
			free((*envp)[setenv->idx]);
			(*envp)[setenv->idx] = setenv->new_val;
		}
		return ;
	}
	setenv->new_val = env_alloc(envp, setenv, 2);
	if (setenv->new_val == NULL)
		free(setenv->key);
	else
	{
		ft_strlcpy(setenv->new_val, (*envp)[setenv->idx], -1);
		ft_strlcat(setenv->new_val, "=", -1);
		ft_strlcat(setenv->new_val, setenv->plus + 2, -1);
		free((*envp)[setenv->idx]);
		(*envp)[setenv->idx] = setenv->new_val;
	}
}

static void	set_env_noindex(char ***envp, char *var_value, t_setenv *setenv)
{
	*setenv->plus = '+';
	setenv->equal = ft_strchr(var_value, '=');
	setenv->key_len = setenv->plus - setenv->key;
	setenv->val_len = ft_strlen(setenv->equal + 1);
	setenv->new_val = malloc(setenv->key_len + setenv->val_len + 2);
	if (setenv->new_val == NULL)
		free(setenv->key);
	else
	{
		ft_strncpy(setenv->new_val, var_value, setenv->plus - setenv->key);
		(setenv->new_val)[setenv->plus - setenv->key] = '=';
		ft_strlcpy(setenv->new_val + (setenv->plus - setenv->key) + 1,
			setenv->equal + 1, -1);
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

	setenv.key = ft_strdup(var_value);
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

static void	export_equal(t_exp exp)
{
	**exp.equal = '\0';
	if (exp.plus)
	{
		*(*exp.equal - 1) = '\0';
		*exp.wstatus = !valid_id_plus(*exp.params[*exp.i], *exp.equal + 1);
	}
	else
		*exp.wstatus = !valid_id(*exp.params[*exp.i], *exp.equal + 1);
	if (!*exp.wstatus)
	{
		**exp.equal = '=';
		if (exp.plus)
		{
			*(*exp.equal - 1) = '+';
			set_env_with_equal_plus(&((*exp.env)->export), *exp.params[*exp.i]);
			set_env_with_equal_plus(&((*exp.env)->envp), *exp.params[*exp.i]);
		}
		else
		{
			set_env_with_equal(&((*exp.env)->export), *exp.params[*exp.i]);
			set_env_with_equal(&((*exp.env)->envp), *exp.params[*exp.i]);
		}
	}
}

void	ft_doexport(t_env *env, char **params, int *wstatus)
{
	int		i;
	char	*equal;

	i = 0;
	while (params[i] != NULL)
	{
		equal = ft_strchr(params[i], '=');
		if (equal != NULL && *(equal - 1) != '+')
			export_equal((t_exp){&env, &params, wstatus, &i, &equal, 0});
		else if (equal == NULL)
		{
			*wstatus = !valid_id(params[i], NULL);
			if (!*wstatus)
				set_env_without_equal(&env->export, params[i]);
		}
		else
			export_equal((t_exp){&env, &params, wstatus, &i, &equal, 1});
		i++;
	}
}
