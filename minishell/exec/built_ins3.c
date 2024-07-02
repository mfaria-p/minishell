/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 21:18:12 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/03 00:35:32 by mfaria-p         ###   ########.fr       */
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

int	is_valid_identifier(char *var, char *value)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[0]) && var[0] != '_')
	{
		error_identifier(var, value);
		return (0);
	}
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
		{
			error_identifier(var, value);
			return (0);
		}
		i++;
	}
	if (!value)
		return (2);
	return (1);
}

/* void	sort_env(char **envp)
{
	int		i;
	int		j;
	char	*temp;
	int		len;
	int		i;

	i = 0;
	j = 0;
	while (envp[i] != NULL)
	{
		j = i + 1;
		while (envp[j] != NULL)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
} */

// Function to find the variable in the environment
int	find_var(char **envp, const char *var)
{
	int	len1;
	int	i;
	char	*equal_sign;
	int	len2;

	len2 = 0;
	equal_sign = ft_strchr(var, '=');
	if (equal_sign != NULL)
	{
		*equal_sign = '\0';
		len2 = ft_strlen(var);
		*equal_sign = '=';
	}
	else
		len2 = ft_strlen(var);
	len1 = 0;
	i = 0;
	while (envp[i] != NULL)
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			len1 = ft_strlen(envp[i]);
			*equal_sign = '=';
		}
		else
			len1 = ft_strlen(envp[i]);
		if (ft_strncmp(envp[i], var, len1) == 0 && (len1 == len2))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

char	*find_oldpwd(char **envp)
{
	while (*envp && ft_strncmp("OLDPWD=", *envp, 7) != 0)
		envp++;
	if (*envp)
		return (*envp + 7);
	return (NULL);
}

char	*create_env_var(const char *var, const char *value)
{
	char	*env_var;
	size_t	var_len;
	size_t	value_len;

	var_len = strlen(var);
	value_len = strlen(value);
	env_var = malloc(var_len + value_len + 2); // +2 for '=' and null terminator
	if (!env_var)
		return (NULL);
	strcpy(env_var, var);
	strcat(env_var, "=");
	strcat(env_var, value);
	return (env_var);
}

void	ft_cd(char ***envp, char *path)
{
	char	*current;
	char	*old_dir;
	char	*oldpwd_var;
	char	*pwd_var;

	old_dir = find_oldpwd(*envp);
	current = find_pwd(*envp);
	if (ft_strlen(path) == 1 && path[0] == '-')
	{
		printf("%s\n", old_dir);
		chdir(old_dir);
	}
	else
		chdir(path);
	if (current)
	{
		oldpwd_var = create_env_var("OLDPWD", current);
		set_env_with_equal(envp, oldpwd_var);
	}
	current = find_pwd(*envp);
	if (current)
	{
		pwd_var = create_env_var("PWD", current);
		set_env_with_equal(envp, pwd_var);
	}
}

char	*find_home(char **envp)
{
	while (*envp && ft_strncmp("HOME=", *envp, 5) != 0)
		envp++;
	if (*envp)
		return (*envp + 5);
	return (NULL);
}

void	ft_cd_home(char **envp)
{
	char	*home;

	home = find_home(envp);
	chdir(home);
}