/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 12:16:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/06/29 14:45:51 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "readline/readline.h"

int	ft_isexit(char *str)
{
	if (str)
		if (*str++ == 'e')
			if (*str++ == 'x')
				if (*str++ == 'i')
					if (*str++ == 't')
						if (*str == '\0')
							return (1);
	return (0);
}

int	main(void)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("( ๑ ˃̵ᴗ˂̵)و ");
		if (ft_isexit(line))
		{
			free(line);
			break ;
		}
		lex(line);
		destroy_tree(print_tree(parse()));
		free(line);
	}
}
