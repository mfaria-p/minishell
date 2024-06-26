/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/26 23:49:06 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// falta error handling no here doc (mb tenho q cleanar o temp file dps?)
// o redirect input e append n ta a dar (da segmentation fault),
//	nem o seu erro handling

void	have_child(struct s_node_pipe *pip, int rw, int pipefd[2], char **envp)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		ft_error(1);
	if (pid == 0)
	{
		dup2(pipefd[rw], rw);
		if (dup2(pipefd[rw], rw) == -1)
			ft_error(2);
		close(pipefd[0]);
		close(pipefd[1]);
		if (rw == PIPE_WRITE)
			execution((struct s_node_default *)pip->left_node, envp);
		else
			execution((struct s_node_default *)pip->right_node, envp);
	}
}

void	exec_pipe(struct s_node_pipe *pip, char **envp)
{
	int	pipefd[2];
	int	pid;

	pipe(pipefd);
	if (pipe(pipefd) == -1)
		ft_error(2);
	have_child(pip, PIPE_WRITE, pipefd, envp);
	have_child(pip, PIPE_READ, pipefd, envp);
	waitpid(-1, NULL, 0);
}

void	exec_red(struct s_node_redirect *red, char **envp)
{
	if (red->node_type == R_out)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_TRUNC, STDOUT_FILENO,
			envp);
	else if (red->node_type == R_app)
		exec_not_heredoc(red, O_CREAT | O_WRONLY, STDOUT_FILENO, envp);
	else if (red->node_type == R_heredoc)
		exec_heredoc(red, envp);
	else
		exec_not_heredoc(red, O_RDONLY, STDIN_FILENO, envp);
	if (red->next)
		execution((struct s_node_default *)red->next, envp);
}

void	exec_exec(struct s_node_execution *exec, char **envp)
{
	if (exec->node_type == E_builtin)
	{
		if (!ft_strncmp(exec->command, "echo", 5))
			ft_echo(exec->params);
		else if (!ft_strncmp(exec->command, "cd", 3))
			ft_cd(envp, exec->params);
		else if (!ft_strncmp(exec->command, "pwd", 4))
			ft_pwd(envp);
		else if (!ft_strncmp(exec->command, "export", 7) && !exec->params)
			ft_printexport(envp);
		else if (!ft_strncmp(exec->command, "export", 7) && exec->params[0])
			ft_doexport(envp, exec->params);
		else if (!ft_strncmp(exec->command, "unset", 6))
			ft_unset(exec->params);
		else if (!ft_strncmp(exec->command, "env", 4))
			ft_printenv(envp);
	}
	else
		ft_execute(exec, envp);
}

void	execution(struct s_node_default *node, char **envp)
{
	if ((node->node_type & E_cmd) || (node->node_type & E_builtin))
		exec_exec((struct s_node_execution *)node, envp);
	else if (node->node_type & (1 << 5))
		exec_red((struct s_node_redirect *)node, envp);
	else
		exec_pipe((struct s_node_pipe *)node, envp);
}
