/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:13:55 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/12 22:44:38 by mfaria-p         ###   ########.fr       */
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
	{
		return (getcwd(cwd, sizeof(cwd)));
	}
	else
		ft_error(6);
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
	// Count the number of environment variables
	while (envp[count] != NULL)
		count++;
	// Allocate space for the new envp array (count
	//	- 1 because we remove one variable)
	new_envp = malloc(count * sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	// Copy variables to new_envp except the one to be removed
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
		printf("%i-", len2);
		printf("%i\t", len1);
		printf("%s-%s\n", var, envp[i]);
		if (len1 == len2 && ft_strncmp(envp[i], var, len2 + 1) == 0)
		{
			i++;
			continue ;
		}
		if (equal_sign != NULL)
			*equal_sign = '=';
		new_envp[j] = envp[i];
		j++;
		i++;
	}
	new_envp[j] = NULL;
	/* while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp); */
	return (new_envp);
}

char	**remove_var_envp(char **envp, char *var)
{
	int		count;
	int		i;
	int		j;
	char	**new_envp;

	count = 0;
	i = 0;
	j = 0;
	// Count the number of environment variables
	while (envp[count] != NULL)
		count++;
	// Allocate space for the new envp array (count
	//	- 1 because we remove one variable)
	new_envp = malloc(count * sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	// Copy variables to new_envp except the one to be removed
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) != 0
			|| envp[i][ft_strlen(var)] != '=')
		{
			new_envp[j] = envp[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	ft_unset(char **args, t_env *env)
{
	int	i;

	i = 0;
	if (!args || !*args)
		return ;
	while (args[i])
	{
		if (find_var(env->envp, args[i]))
		{
			env->envp = remove_var(env->envp, args[i]); // n ta a funceminar
			env->export = remove_var(env->export, args[i]);
		}
		else if (find_var(env->export, args[i]))
		{
			printf("boop");
			env->export = remove_var(env->export, args[i]);
			printf("cabou");
		}
		i++;
	}
}
