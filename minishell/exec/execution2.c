/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <mfaria-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:05:32 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/05 09:12:30 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*find_path(char **envp)
{
	while (*envp && ft_strncmp("PATH=", *envp, 5) != 0)
		envp++;
	if (*envp)
		return (*envp + 5);
	else
		return (NULL);
}

char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	cmd_not_found(cmd);
	return (NULL);
}

char	*find_the_command(char **envp, t_node_e *exec)
{
	char	*paths;
	char	**cmd_paths;
	char	*command;
	int		i;

	i = 0;
	paths = find_path(envp);
	if (!paths)
	{
		file_not_found(exec->command);
		return (NULL);
	}
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

void	ft_execute(t_node_e *exec, t_node_d *root, t_sh sh)
{
	char	*command;
	int		param_count;
	char	**argv;
	int		i;

	i = 0;
	param_count = 0;
	if (access(exec->command, X_OK) == 0)
		command = exec->command;
	else
		command = find_the_command(sh.env->envp, exec);
	if (command && is_regular_file(command))
	{
		while (exec->params && exec->params[param_count] != NULL)
			param_count++;
		argv = (char **)malloc((param_count + 2) * sizeof(char *));
		argv[0] = ft_strdup(command);
		while (i < param_count)
		{
			argv[i + 1] = ft_strdup(exec->params[i]);
			i++;
		}
		argv[param_count + 1] = NULL;
		destroy_tree(root);
		execve(argv[0], argv, sh.env->envp);
	}
	err_msg(command, "couldn't execute\n", sh.stat);
}

int	file_exist(const char *filename)
{
	return (access(filename, F_OK) != -1);
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
