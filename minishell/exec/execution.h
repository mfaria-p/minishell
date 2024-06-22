/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:39:26 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/22 19:05:53 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include "ft_printf/ft_printf.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
# include <stdio.h> 
# include <sys/wait.h>
# include <sys/types.h>
#include <stdarg.h>
//#include <string.h>
//#include <stdio.h>

#define PIPE_READ STDIN_FILENO
#define PIPE_WRITE STDOUT_FILENO
#define MODE 0644

//Nodes structures
struct node_default
{
	int	node_type; //PIPE REDIRECT EXEC
};

struct node_pipe
{
	int	node_type;
	struct node_default *left_node;
	struct node_default *right_node;
};

struct node_redirect
{
	int	node_type;
	char  *filename;
	char  *delimeter;
	struct node_default *next;
};

struct node_execution
{
	int	node_type;
	char  *command;
	char  **params;
};

//Node types
enum e_nodetype
{
	E_cmd = 0b1<<4,
	E_builtin,
	R_out = 0b1<<5,
	R_app,
	R_heredoc,
	R_input,
	P = 0b1<<6
};

//EXECUTION

void  execution(struct node_default *node, char **envp);

// pipe executing functions
void  exec_pipe(struct node_pipe *pip, char **envp);
void  have_child(struct node_pipe *pip , int  rw, int pipefd[2], char **envp);

//redirect executing functions
void  exec_red(struct node_redirect *red, char **envp);
void exec_not_heredoc(struct node_redirect *red, int flags, int io);
void  exec_heredoc(struct node_redirect *red);

//Simple commands executing functions 
void    exec_exec(struct node_execution *exec, char **envp);
char *find_path(char **envp);
char *get_cmd(char **paths, char *cmd);
char    *find_the_command(char **envp, struct node_execution *exec);
void ft_execute(struct node_execution *exec, char **envp);

//utils for finding commands paths
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);

//not done (built ins)
void ft_echo(char **params);
void ft_cd(char **envp, char **params);
void ft_pwd();
void ft_printexport(char **envp);
void ft_doexport(char **envp, char **params);
void ft_unset(char **params);
void ft_printenv(char **envp);
void ft_exit(char **params);

#endif