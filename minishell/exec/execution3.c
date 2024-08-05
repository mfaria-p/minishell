/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:09:16 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/05 09:50:53 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	check_file(char *filename, int io)
{
	if (io == R_input)
		return (access(filename, W_OK) != -1);
	if ((io == R_out || io == R_app) && access(filename, F_OK) != -1)
		return (access(filename, R_OK) != -1);
	if ((io == R_out || io == R_app) && access(filename, F_OK) == -1)
		return (1);
	return (0);
}

void	exec_not_heredoc(t_node_r *red, int flags, int io, t_sh sh)
{
	int	fd;

	*sh.stat = 1;
	if (*(red->filename))
	{
		if (check_file(red->filename, red->type))
		{
			fd = open(red->filename, flags, MODE);
			if (fd == -1 && io == STDIN_FILENO)
				ft_error(4);
			else if (fd == -1 && io == STDOUT_FILENO)
				ft_error(3);
			else if (dup2(fd, io) == -1)
			{
				close(fd);
				ft_error(3);
			}
			*sh.stat = 0;
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
		line = expand(line, 0);
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

pid_t	have_child(t_node_p *pip, int rw, int pipefd[2], t_sh sh)
{
	int	pid;

	pid = fork();
	sh.pid = pid;
	sh.fd = NULL;
	if (pid < 0)
		ft_error(1);
	if (pid == 0)
	{
		sigchild();
		if (dup2(pipefd[rw], rw) == -1)
			ft_error(2);
		close(pipefd[0]);
		close(pipefd[1]);
		if (rw == PIPE_WRITE)
			execution((t_node_d *)pip->lnode, sh);
		else
			execution((t_node_d *)pip->rnode, sh);
		free_env_export(sh.env);
		rl_clear_history();
		exit(*sh.stat);
	}
	sigignore();
	return (pid);
}
