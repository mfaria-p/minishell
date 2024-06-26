/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:13:55 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/26 23:05:00 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// Function to replicate the echo command
void	ft_echo(char **params)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	if (params[0] != NULL && ft_strncmp(params[0], "-n", 3) == 0)
	{
		newline = 0;
		i = 1;
	}
	while (params[i] != NULL)
	{
		if (i > 0)
		{
			printf(" ");
		}
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
void	ft_printexport(char **envp)
{
 /* Implementation for export */
}
void	ft_doexport(char **envp, char **params)
{
 /* Implementation for export */
}
void	ft_unset(char **args)
{ /* Implementation for unset */
}