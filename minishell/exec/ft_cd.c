/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:51:23 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/02 17:36:18 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*find_var2(char *name)
{
	if (getenv(name))
		return (getenv(name));
	else
	{
		error_envp(name);
		return (NULL);
	}
}

char	*create_env_var(const char *var, const char *value)
{
	char	*env_var;
	size_t	var_len;
	size_t	value_len;

	var_len = strlen(var);
	value_len = strlen(value);
	env_var = malloc(var_len + value_len + 2);
	if (!env_var)
		return (NULL);
	strcpy(env_var, var);
	strcat(env_var, "=");
	strcat(env_var, value);
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
	char	current[200];
	char	*old_dir;

	getcwd(current, sizeof(current));
	old_dir = find_oldpwd(env->envp);
	if (ft_strlen(path) == 1 && path[0] == '-')
	{
		printf("%s\n", old_dir);
		if (chdir(old_dir) == -1)
		{
			file_not_found(old_dir);
			*wstatus = 1;
		}
	}
	else if (chdir(path) == -1)
	{
		file_not_found(path);
		*wstatus = 1;
		return ;
	}
	update_oldpwd(env, current);
	if (getcwd(current, sizeof(current)))
		update_pwd(env, current);
}

void	ft_cd_home(t_env *env)
{
	char	*home;
	char	*oldpwd_var;
	char	*home_var;
	char	current[200];

	getcwd(current, sizeof(current));
	home = find_home(env->envp);
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
