/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:51:23 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/04 08:36:04 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*create_env_var(const char *var, const char *value)
{
	char	*env_var;
	size_t	var_len;
	size_t	value_len;

	var_len = ft_strlen(var);
	if (value)
		value_len = ft_strlen(value);
	else
		value_len = 0;
	env_var = malloc(var_len + value_len + 2);
	if (!env_var)
		return (NULL);
	ft_strlcpy(env_var, var, -1);
	ft_strlcat(env_var, "=", -1);
	if (value)
		ft_strlcat(env_var, value, -1);
	return (env_var);
}

char	*find_oldpwd(char **envp)
{
	while (*envp && ft_strncmp("OLDPWD=", *envp, 7) != 0)
		envp++;
	if (*envp)
		return (*envp + 7);
	else
	{
		ft_error(8);
		return (NULL);
	}
}

char	*find_home(char **envp)
{
	while (*envp && ft_strncmp("HOME=", *envp, 5) != 0)
		envp++;
	if (*envp)
		return (*envp + 5);
	else
	{
		err_cd();
		return (NULL);
	}
}

void	ft_cd(t_env *env, char *path, int *wstatus)
{
	char	*current;
	char	*old_dir;
	char	current2[200];

	current = find_pwd(env->envp);
	old_dir = find_oldpwd(env->envp);
	*wstatus = 1;
	if (!old_dir || chdir(old_dir) == -1)
		return ;
	else if (ft_strlen(path) == 1 && path[0] == '-' && old_dir)
	{
		printf("%s\n", old_dir);
		if (chdir(old_dir) == -1)
			file_not_found(old_dir);
		return ;
	}
	else if (chdir(path) == -1 && ft_strlen(path) != 1 && path[0] != '-')
	{
		file_not_found(path);
		return ;
	}
	*wstatus = 0;
	update_oldpwd(env, current);
	if (getcwd(current2, sizeof(current2)))
		update_pwd(env, current2);
}

void	ft_cd_home(t_env *env, int *wstatus)
{
	char	*home;
	char	*oldpwd_var;
	char	*home_var;
	char	*current;

	current = find_pwd(env->envp);
	home = find_home(env->envp);
	if (home)
	{
		chdir(home);
		oldpwd_var = create_env_var("OLDPWD", current);
		home_var = create_env_var("PWD", home);
		set_env_with_equal(&env->envp, oldpwd_var);
		set_env_with_equal(&env->export, oldpwd_var);
		free(oldpwd_var);
		set_env_with_equal(&env->envp, home_var);
		set_env_with_equal(&env->export, home_var);
		free(home_var);
	}
	if (!home)
		*wstatus = 1;
}
