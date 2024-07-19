/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 23:14:24 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/19 18:11:46 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	char	**export;
	char	**envp2;
	t_env	env;
	const char	*temp_file_name = "/tmp/heredoc_tmp";

	siginit();
	env = init_env(&export, &envp2, envp);
	main_loop(&env);
	free_env_export(&env);
	if (env.export)
		printf("not freed");
	else
		printf("freed");
	rl_clear_history();
	unlink(temp_file_name);
}

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

t_env	init_env(char ***export, char ***envp2, char **envp)
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
	env.export = *export;
	*envp2 = malloc((count + 1) * sizeof(char *));
	if (*envp2 == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		(*envp2)[i] = ft_strdup(envp[i]);
		i++;
	}
	(*envp2)[count] = NULL;
	env.envp = *envp2;
	return (env);
}

// Function to free the export array in the environment structure
void	free_env_export(t_env *env)
{
	int	i;

	i = 0;
	if (env->export)
	{
		while (env->export[i] != NULL)
		{
			free(env->export[i]);
			i++;
		}
		free(env->export);
	}
	i = 0;
	if (env->envp)
	{
		while (env->envp[i] != NULL)
		{
			free(env->envp[i]);
			i++;
		}
		free(env->envp);
	}
}

// Function for the main execution loop
void	main_loop(t_env *env)
{
	char	*line;
	t_fds	fds;
	pid_t	pid;

	line = NULL;
	fds.in = dup(STDIN_FILENO);
	fds.out = dup(STDOUT_FILENO);
	while (1)
	{
		line = readline("( ๑ ˃̵ᴗ˂̵)و ");
		if (line)
			add_history(line);
		else
			exit(EXIT_SUCCESS);
		if (ft_isexit(line))
		{
			free(line);
			break ;
		}
		lex(line);
		destroy_tree(execution(parse(), env, 1, &fds));
		waitpid(-1, NULL, 0);
		free(line);
		dup2(fds.in, STDIN_FILENO);
		dup2(fds.out, STDOUT_FILENO);
	}
	close(fds.in);
	close(fds.out);
}
