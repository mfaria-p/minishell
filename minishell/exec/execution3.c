/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:09:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/26 23:10:29 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exec_not_heredoc(struct s_node_redirect *red, int flags, int io,
		char **envp)
{
	if (red->node_type == R_input && file_exist(red->filename))
	{
		have_child_n_hd(red, envp, flags, io);
		waitpid(-1, NULL, 0);
		exit(EXIT_SUCCESS);
	}
	else
		file_not_found(red->filename);
}

void	have_child_n_hd(struct s_node_redirect *red, char **envp, int flags,
		int io)
{
	int	pid;
	int	fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(red->filename, flags, MODE);
		if (fd == -1 && io == STDIN_FILENO)
			ft_error(4);
		else if (fd == -1 && io == STDOUT_FILENO)
			ft_error(3);
		dup2(fd, io);
		if (dup2(fd, io) == -1)
		{
			close(fd);
			ft_error(3);
		}
		close(fd);
		execution((struct s_node_default *)red->next, envp);
		exit(0);
	}
}

// Function to read input for the heredoc and write it to a temporary file
int	create_heredoc(const char *delimiter, const char *file_name)
{
	int		fd;
	char	*line;

	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		ft_error(5);
	line = readline(HEREDOC_MSG);
	while (line && ft_strncmp(delimiter, line, ft_strlen(delimiter)) != 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(HEREDOC_MSG);
	}
	free(line);
	close(fd);
	if (line == NULL)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}

void	have_child_hd(struct s_node_redirect *red, char **envp,
		const char *file_name)
{
	int	pid;
	int	fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(file_name, O_RDONLY);
		if (fd < 0)
			ft_error(5);
		dup2(fd, STDIN_FILENO);
		close(fd);
		execution((struct s_node_default *)red->next, envp);
		exit(0);
	}
}

void	exec_heredoc(struct s_node_redirect *red, char **envp)
{
	const char	*temp_file_name = "/tmp/heredoc_tmp";

	if (create_heredoc(red->delimeter, temp_file_name) == EXIT_SUCCESS)
	{
		have_child_hd(red, envp, temp_file_name);
		waitpid(-1, NULL, 0);
		exit(EXIT_SUCCESS);
		// TENHO QUE CLEANAR O TEMP FILE DPS
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}
