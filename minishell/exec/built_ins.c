/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:13:55 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/17 20:39:57 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_countchar(const char *str, char c)
{
	int	count;

	count = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

// Function to replicate the echo command
void	ft_echo(char **params)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	while (params && params[i] && strncmp(params[i], "-n", 2) == 0
		&& ft_countchar(params[i] + 1, 'n') == (int)(strlen(params[i] + 1)))
	{
		newline = 0;
		i++;
	}
	while (params && params[i] != NULL)
	{
		if (i > 0)
			printf(" ");
		printf("%s", params[i]);
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_pwd(char **envp)
{
	char	cwd[200];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_error(6);
}

char	*find_pwd(char **envp)
{
	char	cwd[200];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (getcwd(cwd, sizeof(cwd)));
	else
		ft_error(6);
	return (NULL);
}

void	ft_printenv(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
}

char	**remove_var(char **envp, char *var)
{
	int		count;
	int		i;
	int		j;
	char	**new_envp;
	int		len2;
	int		len1;
	char	*equal_sign;

	count = 0;
	i = 0;
	j = 0;
	len2 = ft_strlen(var);
	while (envp[count] != NULL)
		count++;
	new_envp = malloc(count * sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	while (envp[i] != NULL)
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			len1 = ft_strlen(envp[i]);
		}
		else
			len1 = ft_strlen(envp[i]);
		if (len1 == len2 && ft_strncmp(envp[i], var, len2 + 1) == 0)
		{
			i++;
			continue ;
		}
		if (equal_sign != NULL)
			*equal_sign = '=';
		new_envp[j] = ft_strdup(envp[i]);
		j++;
		i++;
	}
	new_envp[j] = NULL;
	ft_free(envp);
	return (new_envp);
}

char	**remove_var_envp(char **envp, char *var, int k)
{
	int		count;
	int		i;
	int		j;
	char	**new_envp;
	int		len2;
	int		len1;
	char	*equal_sign;

	count = 0;
	i = 0;
	j = 0;
	len2 = ft_strlen(var);
	while (envp[count] != NULL)
		count++;
	new_envp = malloc(count * sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	while (envp[i] != NULL)
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			len1 = ft_strlen(envp[i]);
		}
		else
			len1 = ft_strlen(envp[i]);
		if (len1 == len2 && ft_strncmp(envp[i], var, len2 + 1) == 0)
		{
			i++;
			continue ;
		}
		if (equal_sign != NULL)
			*equal_sign = '=';
		new_envp[j] = ft_strdup(envp[i]);
		j++;
		i++;
	}
	new_envp[j] = NULL;
	if (k == 1)
		ft_free(envp);
	else
		k == 1;
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
			env->envp = remove_var_envp(env->envp, args[i], env->i);
			env->export = remove_var(env->export, args[i]);
		}
		else if (find_var(env->export, args[i]) != -1)
			env->export = remove_var(env->export, args[i]);
		i++;
	}
}
