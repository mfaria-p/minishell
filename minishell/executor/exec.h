/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 22:57:01 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/20 14:10:17 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

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

typedef struct s_node
{
	int			  type; //REDIRECT PIPE EXEC
	char  		  *content; //REDIRECT -> FILE || EXEC -> COMMAND
	char  		  **params; //EXEC -> ARGUMENTS/OPTIONS
	struct s_node *lnode;
	struct s_node *rnode;
} t_node;

//falta m a strncmp e a ft_split e a str_join

// Function prototypes
void exec_cmdline(t_node *tree, char **envp);
void exec_cmd(t_node *tree, char **envp);
void exec_simple_cmd(t_node *tree, char **envp);
void exec_pipeline(t_node *tree, char **envp);
int ft_isbuiltin(t_node *cmd);

//not done (built ins)
void ft_echo(char **args);
void ft_cd(char *path);
void ft_pwd();
void ft_printexport();
void ft_doexport(char **args);
void ft_unset(char **args);
void ft_printenv();
void ft_exit(char **args);

//execute other simple commands with the path
char *get_cmd(char **paths, char *cmd);
char *find_path(char **envp);
void ft_execute(t_node *tree, char **envp);

//utils, aka ft_split, ft_strncmp and ft_strjoin
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);

#endif