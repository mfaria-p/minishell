/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 00:02:02 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/02 16:50:49 by ecorona-         ###   ########.fr       */
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
