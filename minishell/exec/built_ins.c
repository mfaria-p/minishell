/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:13:55 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/30 18:25:29 by mfaria-p         ###   ########.fr       */
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
	while (params && params[i] && strncmp(params[i], "-n", 2) == 0 && \
		ft_countchar(params[i] + 1, 'n') == (int)(strlen(params[i] + 1)))
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

char	*find_pwd(char **envp)
{
	while (*envp && ft_strncmp("PWD=", *envp, 4) != 0)
		envp++;
	if (*envp)
		return (*envp + 4);
	return (NULL);
}

void	ft_pwd(char **envp)
{
	char	*pwd;

	pwd = find_pwd(envp);
	if (pwd)
		printf("%s\n", pwd);
	else
		ft_error(6);
	exit(EXIT_SUCCESS);
}

void	ft_printenv(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
}

void	ft_cd(char **envp, char **params)
{
	/* Implementation for cd */
}

void	ft_unset(char **args)
{
 /* Implementation for unset */
}
