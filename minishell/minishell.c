/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 23:14:24 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/30 21:34:01 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_env	init_env(char ***export, char **envp)
{
	t_env	env;
	int		count;
	int		i;

	count = 0;
	while (envp[count] != NULL)
		count++;
	*export = malloc((count + 1) * sizeof(char *));
	if (*export == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		(*export)[i] = ft_strdup(envp[i]);
		i++;
	}
	(*export)[count] = NULL;
	env.envp = envp;
	env.export = *export;
	return (env);
}

// Function to free the export array in the environment structure
void	free_env_export(t_env *env)
{
	int	i;

	i = 0;
	while (env->export[i] != NULL)
	{
		free(env->export[i]);
		i++;
	}
	free(env->export);
}

// Function for the main execution loop
void	main_loop(t_env *env)
{
	char	*line;
	int		fd[2];

	line = NULL;
	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	while (1)
	{
		line = readline("( ๑ ˃̵ᴗ˂̵)و ");
		if (line)
			add_history(line);
		if (ft_isexit(line))
		{
			free(line);
			break ;
		}
		lex(line);
		/*destroy_tree(execution(parse(), env));*/
		execution(parse(), env);
		waitpid(-1, NULL, 0);
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**export;
	t_env	env;

	env = init_env(&export, envp);
	main_loop(&env);
	free_env_export(&env);
	rl_clear_history();
}
