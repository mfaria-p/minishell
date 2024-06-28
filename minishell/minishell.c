/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 23:14:24 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/28 17:38:26 by mfaria-p         ###   ########.fr       */
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
	t_token	tok;
	pid_t	pid;

	line = NULL;
	while (1)
	{
		line = readline("( ๑ ˃̵ᴗ˂̵)و ");
		if (*line)
			add_history(line);
		if (ft_isexit(line))
		{
			free(line);
			break ;
		}
		tok = lex(line);
		pid = fork();
		if (pid == 0)
		{
			execution(parse(line), env);
			exit(EXIT_SUCCESS);
		}
		waitpid(-1, NULL, 0);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**export;
	char	*line;
	t_token	tok;
	pid_t	pid;
	t_env	env;

	env = init_env(&export, envp);
	line = NULL;
	main_loop(&env);
	free_env_export(&env);
	line = NULL;
	tok = (t_token){0, NULL};
}
