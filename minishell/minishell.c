#include "minishell.h"
#include <complex.h>

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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	tok;
	pid_t	pid;

	line = NULL;
	while (1)
	{
		line = readline("( ๑ ˃̵ᴗ˂̵)و ");
		if (ft_isexit(line))
		{
			free(line);
			break ;
		}
		tok = lex(line);
		pid = fork();
		if (pid == 0)
		{
			execution(parse(line), envp);
			exit(EXIT_SUCCESS);
		}
		waitpid(-1, NULL, 0);
		free(line);
	}
	line = NULL;
	tok = (t_token){0, NULL};
}
