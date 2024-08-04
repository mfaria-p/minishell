/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:03:01 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/04 00:48:05 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_pwd(char **envp)
{
	char *pwd;
	pwd = find_pwd(envp);
	if (pwd)
		printf("%s\n", pwd);
	else
		ft_error(6);
}

char	*find_pwd(char **envp)
{
	while (*envp && ft_strncmp("PWD=", *envp, 4) != 0)
		envp++;
	if (*envp)
		return (*envp + 4);
	else
		return (NULL);
}
