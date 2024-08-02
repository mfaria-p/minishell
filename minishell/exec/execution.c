/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/02 19:57:33 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>
#include <unistd.h>

// falta error handling no here doc (mb tenho q cleanar o temp file dps?)
// o redirect input e append n ta a dar (da segmentation fault),
// nem o seu erro handling

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
	return (pid);
}

void	exec_pipe(t_node_p *pip, t_sh sh)
{
	int		pipefd[2];
	pid_t	pid[2];

	if (pipe(pipefd) == -1)
		ft_error(2);
	pid[0] = have_child(pip, PIPE_WRITE, pipefd, sh);
	pid[1] = have_child(pip, PIPE_READ, pipefd, sh);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], sh.stat, 0);
	*sh.stat = WEXITSTATUS(*sh.stat);
}

void	exec_red(t_node_r *red, t_sh sh)
{
	int	out;

	out = O_CREAT | O_WRONLY;
	*sh.stat = 0;
	sh.fd = NULL;
	if (red->type == R_out)
		exec_not_heredoc(red, out | O_TRUNC, STDOUT_FILENO, sh);
	else if (red->type == R_app)
		exec_not_heredoc(red, out | O_APPEND, STDOUT_FILENO, sh);
	else if (red->type == R_heredoc)
		exec_heredoc(red, sh);
	else
		exec_not_heredoc(red, O_RDONLY, STDIN_FILENO, sh);
	if (red->next && (*(red->filename)) && red->filename != NULL && !*sh.stat)
		execution((t_node_d *)red->next, sh);
}

void	exec_exec(t_node_e *exec, t_node_d *root, t_fds *fd, t_sh sh)
{
	pid_t	pid;

	if (!ft_strncmp(exec->command, "exit", 5))
		ft_exit(exec->params, sh.stat);
	else if (!ft_strncmp(exec->command, "echo", 5))
		ft_echo(exec->params);
	else if (!ft_strncmp(exec->command, "cd", 3) && !exec->params)
		ft_cd_home(sh.env);
	else if (!ft_strncmp(exec->command, "cd", 3) && exec->params[0])
	{
		ft_cd(sh.env, exec->params[0], sh.stat);
		if (exec->params[1])
		{
			write(STDERR_FILENO ,"minishell: cd: too many arguments\n", 35);
			*sh.stat = 1;
		}
	}
	else if (!ft_strncmp(exec->command, "pwd", 4))
		ft_pwd(sh.env->envp);
	else if (!ft_strncmp(exec->command, "export", 7) && !exec->params)
		ft_printexport(sh.env->export);
	else if (!ft_strncmp(exec->command, "export", 7) && exec->params[0])
		ft_doexport(sh.env, exec->params, sh.stat);
	else if (!ft_strncmp(exec->command, "unset", 6))
		ft_unset(exec->params, sh.env);
	else if (!ft_strncmp(exec->command, "env", 4))
		ft_printenv(sh.env->envp);
	else if (!sh.pid)
		ft_execute(exec, root, sh);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			close(fd->in);
			close(fd->out);
			ft_execute(exec, root, sh);
			destroy_tree(root);
			free_env_export(sh.env);
			exit(*sh.stat);
		}
		waitpid(pid, sh.stat, 0);
		*sh.stat = WEXITSTATUS(*sh.stat);
	}
}

t_node_d	*execution(t_node_d *node, t_sh sh)
{
	static t_node_d	*root;
	static t_fds	*fd;

	if (!root)
		root = node;
	if (!fd)
		fd = sh.fd;
	if (node)
	{
		if ((node->type & E_cmd))
			exec_exec((t_node_e *)node, root, fd, sh);
		else if (node->type & (1 << 5))
			exec_red((t_node_r *)node, sh);
		else
			exec_pipe((t_node_p *)node, sh);
	}
	if (!sh.pid)
	{
		close(fd->in);
		close(fd->out);
		destroy_tree(root);
	}
	root = NULL;
	fd = NULL;
	return (node);
}
