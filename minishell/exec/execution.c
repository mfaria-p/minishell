/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/19 18:17:29 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>
#include <unistd.h>

// falta error handling no here doc (mb tenho q cleanar o temp file dps?)
// o redirect input e append n ta a dar (da segmentation fault),
// nem o seu erro handling

pid_t	have_child(struct s_node_pipe *pip, int rw, int pipefd[2], t_env *env)
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
			execution((struct s_node_default *)pip->left_node, env, pid, NULL);
		else
			execution((struct s_node_default *)pip->right_node, env, pid, NULL);
		free_env_export(env);
		exit(EXIT_SUCCESS);
	}
	return (pid);
}

void	exec_pipe(struct s_node_pipe *pip, t_env *env)
{
	int		pipefd[2];
	pid_t	pid[2];

	if (pipe(pipefd) == -1)
		ft_error(2);
	pid[0] = have_child(pip, PIPE_WRITE, pipefd, env);
	pid[1] = have_child(pip, PIPE_READ, pipefd, env);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
}

void	exec_red(struct s_node_redirect *red, t_env *env, pid_t is_parent)
{
	if (red->node_type == R_out)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_TRUNC, STDOUT_FILENO, env);
	else if (red->node_type == R_app)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_APPEND, STDOUT_FILENO, env);
	else if (red->node_type == R_heredoc)
		exec_heredoc(red, env);
	else
		exec_not_heredoc(red, O_RDONLY, STDIN_FILENO, env);
	if (red->next)
		execution((struct s_node_default *)red->next, env, is_parent, NULL);
}

void	exec_exec(struct s_node_execution *exec, t_env *env, pid_t is_parent)
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
		ft_execute(exec, env->envp);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			ft_execute(exec, env->envp);
			free_env_export(env);
			exit(EXIT_SUCCESS);
		}
		waitpid(pid, NULL, 0);
	}
}

t_node_default	*execution(struct s_node_default *node, t_env *env, pid_t is_parent, t_fds *fd)
{
	if ((node->node_type & E_cmd))
		exec_exec((struct s_node_execution *)node, env, is_parent);
	else if (node->node_type & (1 << 5))
		exec_red((struct s_node_redirect *)node, env, is_parent);
	else
		exec_pipe((struct s_node_pipe *)node, env);
	return (node);
}
