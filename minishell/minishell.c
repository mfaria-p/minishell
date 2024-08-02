/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 23:14:24 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/02 21:06:43 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	char		**export;
	char		**envp2;
	t_env		env;
	const char	*temp_file_name = "/tmp/heredoc_tmp";
	int			exit_status;

	env = init_env(&export, &envp2, envp);
	exit_status = main_loop(&env);
	free_env_export(&env);
	rl_clear_history();
	unlink(temp_file_name);
	return (exit_status);
}

int	ft_isexit(char *str)
{
	if (!ft_strncmp(str, "exit", 4) && (ft_isspace(*(str + 4)) || !*(str + 4)))
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
int	main_loop(t_env *env)
{
	char		*line;
	t_fds		fd;
	static int	stat;

	line = NULL;
	fd.in = dup(STDIN_FILENO);
	fd.out = dup(STDOUT_FILENO);
	while (1)
	{
		g_sig = 0;
		siginit();
		line = readline("( ๑ ˃̵ᴗ˂̵)و ");
		if (g_sig)
			stat = g_sig + 128;
		if (line)
		{
			if (strlen(line) > 0)
			{
				add_history(line);
				if (ft_isexit(line))
				{
					lex(line, &stat);
					destroy_tree(execution(parse(), (t_sh){env, 1, &fd, &stat}));
					dup2(fd.in, STDIN_FILENO);
					dup2(fd.out, STDOUT_FILENO);
					free(line);
					break ;
				}
				lex(line, &stat);
				destroy_tree(execution(parse(), (t_sh){env, 1, &fd, &stat}));
				dup2(fd.in, STDIN_FILENO);
				dup2(fd.out, STDOUT_FILENO);
			}
			free(line);
		}
		else
			exit(EXIT_FAILURE);
	}
	close(fd.in);
	close(fd.out);
	return (stat);
}
