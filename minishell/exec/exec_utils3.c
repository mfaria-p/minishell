/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:14:11 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/27 15:53:39 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_isalpha(int c)
{
	return (1024 * ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)));
}

//ft_strchr

//ft_strcpy