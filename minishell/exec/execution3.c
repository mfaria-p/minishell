/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:09:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/03 17:16:50 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exec_not_heredoc(t_node_r *red, int flags, int io, t_sh sh)
{
	int	fd;

	if (*(red->filename))
	{
		if ((red->type == R_input && file_exist(red->filename)) \
			|| red->type == R_out || red->type == R_app)
		{
			fd = open(red->filename, flags, MODE);
			if (fd == -1 && io == STDIN_FILENO)
			{
				ft_error(4);
				*sh.stat = 1;
				return ;
			}
			else if (fd == -1 && io == STDOUT_FILENO)
			{
				ft_error(3);
				*sh.stat = 1;
				return ;
			}
			if (dup2(fd, io) == -1)
			{
				close(fd);
				ft_error(3);
				*sh.stat = 1;
				return ;
			}
			close(fd);
		}
		else
		{
			file_not_found(red->filename);
			*sh.stat = 1;
		}
	}
	else
	{
		ft_error(9);
		*sh.stat = 1;
	}
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

void	exec_heredoc(t_node_r *red, t_sh sh)
{
	const char	*temp_file_name = "/tmp/heredoc_tmp";
	int			fd;

	(void)red;
	if (access(temp_file_name, F_OK) == EXIT_SUCCESS)
	{
		fd = open(temp_file_name, O_RDONLY);
		if (fd < 0)
		{
			ft_error(5);
			*sh.stat = 1;
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}
