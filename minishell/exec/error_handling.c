/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 23:47:45 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/18 19:16:19 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	ft_error(int error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error == 1)
		ft_putstr_fd("Failed to fork\n", STDERR_FILENO);
	else if (error == 2)
		ft_putstr_fd("Failed to create pipe\n", STDERR_FILENO);
	else if (error == 6)
		ft_putstr_fd("outfile: Error\n", STDERR_FILENO);
	else if (error == 7)
		ft_putstr_fd("infile: No such file or directory\n", STDERR_FILENO);
	else if (error == 5)
		ft_putstr_fd("Heredoc: Error\n", STDERR_FILENO);
	else if (error == 6)
		ft_putstr_fd("PWD not found in environment variables.\n",
			STDERR_FILENO);
	else if (error == 7)
		ft_putstr_fd("Path not found in environment variables.\n",
			STDERR_FILENO);
	else if (error == 8)
		ft_putstr_fd("OLDPWD not found in environment variables.\n",
			STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	cmd_not_found(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (127);
}

int	file_not_found(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(":  No such file or directory\n", STDERR_FILENO);
	return (127);
}

int	error_envp(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("not found in environment variables.\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}
