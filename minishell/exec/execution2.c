/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:05:32 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/28 17:16:19 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*find_path(char **envp)
{
	while (*envp && ft_strncmp("PATH=", *envp, 5) != 0)
		envp++;
	if (*envp)
		return (*envp + 5);
	return (NULL);
}

char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		paths++;
	}
	cmd_not_found(cmd);
	return (NULL);
}

char	*find_the_command(char **envp, struct s_node_execution *exec)
{
	char	*paths;
	char	**cmd_paths;
	char	*command;
	int		i;

	i = 0;
	paths = find_path(envp);
	if (!paths)
		ft_error(7);
	cmd_paths = ft_split(paths, ':');
	command = get_cmd(cmd_paths, exec->command);
	while (cmd_paths[i])
	{
		free(cmd_paths[i]);
		i++;
	}
	free(cmd_paths);
	return (command);
}

void	ft_execute(struct s_node_execution *exec, char **envp)
{
	char	*command;
	int		param_count;
	char	**argv;
	int		i;

	i = 0;
	param_count = 0;
	command = find_the_command(envp, exec);
	while (exec->params && exec->params[param_count] != NULL)
		param_count++;
	argv = (char **)malloc((param_count + 2) * sizeof(char *));
	argv[0] = command;
	while (i < param_count)
	{
		argv[i + 1] = exec->params[i];
		i++;
	}
	argv[param_count + 1] = NULL;
	execve(command, argv, envp);
	free(command);
	free(argv);
}

int	file_exist(const char *filename)
{
	return (access(filename, F_OK) != -1);
}
