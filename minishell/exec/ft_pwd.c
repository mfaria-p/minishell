/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:03:01 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/03 16:41:56 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_pwd(char **envp)
{
	char	cwd[200];

	(void)envp;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_error(6);
}

char	*find_pwd(char **envp)
{
	char	cwd[200];

	(void)envp;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (getcwd(cwd, sizeof(cwd)));
	else
		ft_error(6);
	return (NULL);
}
