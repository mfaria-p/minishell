/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/05 11:05:49 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>

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
	if (red->next && red->filename != NULL && (*(red->filename)) && !*sh.stat)
		execution((t_node_d *)red->next, sh);
}

static int	exec_exec_aux(t_node_e *exec, t_sh sh)
{
	if (!ft_strncmp(exec->command, "exit", 5))
		ft_exit(exec->params, sh.stat);
	else if (!ft_strncmp(exec->command, "echo", 5))
		ft_echo(exec->params);
	else if (!ft_strncmp(exec->command, "cd", 3) && !exec->params)
		ft_cd_home(sh.env, sh.stat);
	else if (!ft_strncmp(exec->command, "cd", 3) && exec->params[0])
	{
		ft_cd(sh.env, exec->params[0], sh.stat);
		if (exec->params[1])
		{
			write(STDERR_FILENO, "minishell: cd: too many arguments\n", 35);
			*sh.stat = 1;
		}
	}
	else if (!ft_strncmp(exec->command, "pwd", 4))
		ft_pwd(sh.env->envp);
	else
		return (1);
	return (0);
}

static void	exec_exec_child(t_node_e *exec, t_node_d *root, t_fds *fd, t_sh sh)
{
	sigchild();
	fd_close(fd);
	ft_execute(exec, root, sh);
	destroy_tree(root);
	free_env_export(sh.env);
	exit(*sh.stat);
}

void	exec_exec(t_node_e *exec, t_node_d *root, t_fds *fd, t_sh sh)
{
	pid_t	pid;

	if (exec_exec_aux(exec, sh) == 0)
		return ;
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
			exec_exec_child(exec, root, fd, sh);
		ft_sigignore();
		waitpid(pid, sh.stat, 0);
		child_signal(*sh.stat);
		*sh.stat = WEXITSTATUS(*sh.stat);
	}
}

t_node_d	*execution(t_node_d *node, t_sh sh)
{
	static t_node_d	*root;
	static t_fds	*fd;

	if (!root)
		root = node;
	sh.root = root;
	if (!fd)
		fd = sh.fd;
	if (node)
	{
		*sh.stat = 0;
		if ((node->type & E_cmd))
			exec_exec((t_node_e *)node, root, fd, sh);
		else if (node->type & (1 << 5))
			exec_red((t_node_r *)node, sh);
		else
			exec_pipe((t_node_p *)node, sh);
	}
	if (!sh.pid)
		end_exec(fd, root);
	root = NULL;
	fd = NULL;
	return (node);
}
