#include "minishell.h"

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("8=D ");
		printf("%s\n", line);
		free(line);
	}
	return (0);
}
