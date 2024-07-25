/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:09:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/25 12:02:15 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exec_not_heredoc(struct s_node_redirect *red, int flags, int io,
		t_env *env)
{
	int	fd;
	if (*(red->filename))
	{
		if ((red->node_type == R_input && file_exist(red->filename)) \
			|| red->node_type == R_out || red->node_type == R_app)
		{
			fd = open(red->filename, flags, MODE);
			if (fd == -1 && io == STDIN_FILENO)
			{
				ft_error(4);
				return ;
			}
			else if (fd == -1 && io == STDOUT_FILENO)
			{
				ft_error(3);
				return ;
			}
			if (dup2(fd, io) == -1)
			{
				close(fd);
				ft_error(3);
				return ;
			}
			close(fd);
		}
		else
			file_not_found(red->filename);
	}
	else
		ft_error(9);
}

// Function to read input for the heredoc and write it to a temporary file
int	create_heredoc(const char *delimiter, const char *file_name)
{
	int		fd;
	char	*line;

	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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

void	exec_heredoc(struct s_node_redirect *red, t_env *env)
{
	const char	*temp_file_name = "/tmp/heredoc_tmp";
	int			fd;

	if (access(temp_file_name, F_OK) == EXIT_SUCCESS)
	{
		fd = open(temp_file_name, O_RDONLY);
		if (fd < 0)
			ft_error(5);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}
