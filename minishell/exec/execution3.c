/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:09:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/13 12:51:30 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exec_not_heredoc(struct s_node_redirect *red, int flags, int io,
		t_env *env)
{
	int	fd;

	if ((red->node_type == R_input && file_exist(red->filename)) \
		|| red->node_type == R_out || red->node_type == R_app)
	{
		fd = open(red->filename, flags, MODE);
		if (fd == -1 && io == STDIN_FILENO)
			ft_error(4);
		else if (fd == -1 && io == STDOUT_FILENO)
			ft_error(3);
		if (dup2(fd, io) == -1)
		{
			close(fd);
			ft_error(3);
		}
		close(fd);
	}
	else
		file_not_found(red->filename);
}

/* void	have_child_n_hd(struct s_node_redirect *red, t_env *env, int flags,
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
		execution((struct s_node_default *)red->next, env);
		exit(0);
	}
} */

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
	/* if (line == NULL)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS); */
}

/* void	have_child_hd(struct s_node_redirect *red, t_env *env,
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
		execution((struct s_node_default *)red->next, env);
		exit(0);
	}
} */

void	exec_heredoc(struct s_node_redirect *red, t_env *env)
{
	const char	*temp_file_name = "/tmp/heredoc_tmp";
	int			fd;

	if (create_heredoc(red->delimeter, temp_file_name) == EXIT_SUCCESS)
	{
		fd = open(temp_file_name, O_RDONLY);
		if (fd < 0)
			ft_error(5);
		dup2(fd, STDIN_FILENO);
		close(fd);
		// TENHO QUE CLEANAR O TEMP FILE DPS
	}
	else
		exit(EXIT_FAILURE);
}
