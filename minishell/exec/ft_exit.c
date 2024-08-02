/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 18:07:20 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/02 18:19:24 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void  ft_exit(char **params, int *wstatus)
{
	*wstatus = 0;
	if (params)
	{
		if (params[1])
		{
			*wstatus = 1;
			write(STDERR_FILENO, "minishell: exit: too many arguments\n", 37);
		}
		else if (!ft_isdigit(*params[0]) && (*params[0] != '+') && (*params[0] != '-'))
		{
			*wstatus = 2;
			write(STDERR_FILENO, "minishell: exit: numeric argument required\n", 44);
		}
		else
			*wstatus = ft_atoi(params[0]) % 256;
	}
}
