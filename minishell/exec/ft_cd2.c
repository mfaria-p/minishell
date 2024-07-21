/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:40:14 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/18 19:42:26 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	update_pwd(t_env *env, const char *current)
{
	char	*pwd_var;

	pwd_var = create_env_var("PWD", current);
	set_env_with_equal(&env->envp, pwd_var);
	set_env_with_equal(&env->export, pwd_var);
	free(pwd_var);
}

void	update_oldpwd(t_env *env, const char *current)
{
	char	*oldpwd_var;

	oldpwd_var = create_env_var("OLDPWD", current);
	set_env_with_equal(&env->envp, oldpwd_var);
	set_env_with_equal(&env->export, oldpwd_var);
	free(oldpwd_var);
}
