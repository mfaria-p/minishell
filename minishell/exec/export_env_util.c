/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:19:08 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/05 10:20:16 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	*env_alloc(char ***envp, t_setenv *setenv, int p)
{
	int	size;

	size = ft_strlen((*envp)[setenv->idx]) + ft_strlen(setenv->plus + 2);
	return (malloc(size + p));
}
