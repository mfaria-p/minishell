/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/29 19:56:42 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>
#include <unistd.h>

// falta error handling no here doc (mb tenho q cleanar o temp file dps?)
// o redirect input e append n ta a dar (da segmentation fault),
// nem o seu erro handling

pid_t	have_child(struct s_node_pipe *pip, int rw, int pipefd[2], t_env *env, int *wstatus)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		ft_error(1);
	if (pid == 0)
	{
		if (dup2(pipefd[rw], rw) == -1)
			ft_error(2);
		close(pipefd[0]);
		close(pipefd[1]);
		if (rw == PIPE_WRITE)
			execution((struct s_node_default *)pip->left_node, env, pid, NULL, wstatus);
		else
			execution((struct s_node_default *)pip->right_node, env, pid, NULL, wstatus);
		free_env_export(env);
		exit(EXIT_SUCCESS);
	}
	return (pid);
}

void	exec_pipe(struct s_node_pipe *pip, t_env *env, int *wstatus)
{
	int		pipefd[2];
	pid_t	pid[2];

	if (pipe(pipefd) == -1)
		ft_error(2);
	pid[0] = have_child(pip, PIPE_WRITE, pipefd, env, wstatus);
	pid[1] = have_child(pip, PIPE_READ, pipefd, env, wstatus);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], wstatus, 0);
}

void	exec_red(struct s_node_redirect *red, t_env *env, pid_t is_parent, int *wstatus)
{
	*wstatus = 0;
	if (red->node_type == R_out)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_TRUNC, STDOUT_FILENO, env);
	else if (red->node_type == R_app)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_APPEND, STDOUT_FILENO, env);
	else if (red->node_type == R_heredoc)
		exec_heredoc(red, env);
	else
		exec_not_heredoc(red, O_RDONLY, STDIN_FILENO, env);
	if (red->next && (*(red->filename)) && red->filename != NULL)
		execution((struct s_node_default *)red->next, env, is_parent, NULL, wstatus);
}

void	exec_exec(struct s_node_execution *exec, t_env *env, pid_t is_parent, t_node_default *root, t_fds *fds, int *wstatus)
{
	pid_t	pid;

	if (!ft_strncmp(exec->command, "echo", 5))
		ft_echo(exec->params);
	else if (!ft_strncmp(exec->command, "cd", 3) && !exec->params)
		ft_cd_home(env);
	else if (!ft_strncmp(exec->command, "cd", 3) && exec->params[0])
	{
		ft_cd(env, exec->params[0]);
		if (exec->params[1])
			printf("minishell: cd: too many arguments\n");
	}
	else if (!ft_strncmp(exec->command, "pwd", 4))
		ft_pwd(env->envp);
	else if (!ft_strncmp(exec->command, "export", 7) && !exec->params)
		ft_printexport(env->export);
	else if (!ft_strncmp(exec->command, "export", 7) && exec->params[0])
		ft_doexport(env, exec->params);
	else if (!ft_strncmp(exec->command, "unset", 6))
		ft_unset(exec->params, env);
	else if (!ft_strncmp(exec->command, "env", 4))
		ft_printenv(env->envp);
	else if (!is_parent)
		ft_execute(exec, env->envp, root);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			close(fds->in);
			close(fds->out);
			ft_execute(exec, env->envp, root);
			destroy_tree(root);
			free_env_export(env);
			exit(EXIT_SUCCESS);
		}
		waitpid(pid, wstatus, 0);
	}
}

t_node_default	*execution(struct s_node_default *node, t_env *env, pid_t is_parent, t_fds *fd, int *wstatus)
{
	static t_node_default	*root;
	static t_fds			*fds;

	if (!root)
		root = node;
	if (!fds)
		fds = fd;
	if (node)
	{
		if ((node->node_type & E_cmd))
			exec_exec((struct s_node_execution *)node, env, is_parent, root, fds, wstatus);
		else if (node->node_type & (1 << 5))
			exec_red((struct s_node_redirect *)node, env, is_parent, wstatus);
		else
			exec_pipe((struct s_node_pipe *)node, env, wstatus);
	}
	if (!is_parent)
	{
		close(fds->in);
		close(fds->out);
		destroy_tree(root);
	}
	root = NULL;
	fds = NULL;
	return (node);
}
