/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:39:26 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/26 14:19:56 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include "../minishell.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
# include <stdio.h> 
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/readline.h>
//#include <string.h>
//#include <stdio.h>

#define PIPE_READ STDIN_FILENO
#define PIPE_WRITE STDOUT_FILENO
#define MODE 0644
#define HEREDOC_MSG "> "

/*
//Nodes structures
struct s_node_default
{
	int	s_node_type; //PIPE REDIRECT EXEC
};

struct s_node_pipe
{
	int	s_node_type;
	struct s_node_default *left_node;
	struct s_node_default *right_node;
};

struct s_node_redirect
{
	int	s_node_type;
	char  *filename;
	char  *delimeter;
	struct s_node_default *next;
};

struct s_node_execution
{
	int	s_node_type;
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
*/

//EXECUTION

void  execution(struct s_node_default *node, char **envp);

// pipe executing functions
void  exec_pipe(struct s_node_pipe *pip, char **envp);
void  have_child(struct s_node_pipe *pip , int  rw, int pipefd[2], char **envp);

//redirect executing functions
void  exec_red(struct s_node_redirect *red, char **envp);
void exec_not_heredoc(struct s_node_redirect *red, int flags, int io, char **envp);
void  exec_heredoc(struct s_node_redirect *red, char **envp);
void have_child_hd(struct s_node_redirect *red, char **envp, const char *file_name);
int create_heredoc(const char *delimiter, const char *file_name);

//Simple commands executing functions 
void    exec_exec(struct s_node_execution *exec, char **envp);
char *find_path(char **envp);
char *get_cmd(char **paths, char *cmd);
char    *find_the_command(char **envp, struct s_node_execution *exec);
void ft_execute(struct s_node_execution *exec, char **envp);

//utils for finding commands paths
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_memcpy(void *dest, const void *src, size_t n);

//not done (built ins)
void ft_echo(char **params);
void ft_cd(char **envp, char **params);
void ft_pwd();
void ft_printexport(char **envp);
void ft_doexport(char **envp, char **params);
void ft_unset(char **params);
void ft_printenv(char **envp);
void ft_exit(char **params);

//Error_handling
int	ft_error(int error);
int cmd_not_found(char *str);
void	ft_putstr_fd(char *s, int fd);

#endif
