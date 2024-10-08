/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:43:04 by ecorona-          #+#    #+#             */
/*   Updated: 2024/08/05 11:02:35 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
/* ************************************************************************** */
// MISSING FUNCTIONS
// rl_clear_history
// rl_on_new_line
// rl_replace_line
// rl_redisplay
// add_history
// tgetent
// tgetflag
// tgetnum
// tgetstr
// tgoto
// tputs
/* ************************************************************************** */
# include <unistd.h>
// write
// access
// read
// close
// fork
// getcwd
// chdir
// stat
// lstat
// fstat
// unlink
// execve
// dup
// dup2
// pipe
// isatty
// ttyname
// ttyslot
// tcsetattr
// tcgetattr
/* ************************************************************************** */
# include <stdlib.h>
// malloc
// free
// exit
// getenv
/* ************************************************************************** */
# include <stdio.h>
// readline
// printf
// perror
/* ************************************************************************** */
# include <fcntl.h>
// open
/* ************************************************************************** */
# include <signal.h>
// signal
// sigaction
// sigemptyset
// sigaddset
// kill
/* ************************************************************************** */
# include <dirent.h>
// opendir
// readdir
// closedir
/* ************************************************************************** */
# include <string.h>
// strerror
/* ************************************************************************** */
# include <termios.h>
// tcsetattr
// tcgetattr
/* ************************************************************************** */
# include <readline/readline.h>
// readline
/* ************************************************************************** */
# include <readline/history.h>
// readline
/* ************************************************************************** */
# include <sys/types.h>
// open
// fork
// wait
// waitpid
// wait3
// wait4
// kill
// stat
// lstat
// fstat
// opendir
// closedir
/* ************************************************************************** */
# include <sys/stat.h>
// open
// stat
// lstat
// fstat
/* ************************************************************************** */
# include <sys/wait.h>
// wait
// waitpid
// wait3
// wait4
/* ************************************************************************** */
# include <sys/time.h>
// wait3
// wait4
/* ************************************************************************** */
# include <sys/resource.h>
// wait3
// wait4
/* ************************************************************************** */
# include <sys/ioctl.h>
// ioctl
/* ************************************************************************** */

extern int	g_sig;

typedef struct s_token
{
	int		code;
	char	*content;
	char	unexpected;
}	t_token;

typedef struct s_node_d
{
	int	type;
}	t_node_d;

typedef struct s_node_p
{
	int			type;
	t_node_d	*lnode;
	t_node_d	*rnode;
}	t_node_p;

typedef struct s_node_r
{
	int			type;
	char		*filename;
	char		*delimeter;
	t_node_d	*next;
}	t_node_r;

typedef struct s_node_e
{
	int		type;
	char	*command;
	int		n_params;
	char	**params;
}	t_node_e;

// Node types
enum	e_nodetype
{
	E_cmd = 1 << 4,
	R_out = 1 << 5,
	R_app,
	R_heredoc,
	R_input,
	P = 1 << 6
};

typedef struct s_env
{
	char	**envp;
	char	**export;
}	t_env;

typedef struct s_fds
{
	int	in;
	int	out;
}	t_fds;

typedef struct s_shell
{
	t_node_d	*root;
	t_env		*env;
	pid_t		pid;
	t_fds		*fd;
	int			*stat;
}	t_sh;

t_node_d	*execution(t_node_d *node, t_sh sh);
int			create_heredoc(const char *delimiter, const char *file_name);
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *s);

/* ************************************************************************** */
// lexer + parser
char		*expand(char *str, int status);
int			ft_isspace(int c);
t_token		lex(char *str, int *wstatus);
t_node_d	*parse(int *wstatus);
void		destroy_tree(t_node_d *node);

/* ************************************************************************** */
// signal.c
int			siginit(void);
void		ft_sigignore(void);
void		sigchild(void);
void		child_signal(int wstatus);

/* ************************************************************************** */
// env.c
int			find_var(char **envp, const char *var);
char		*ft_strndup(const char *s, size_t n);
t_env		init_env(char ***export, char ***envp2, char **envp);
void		free_env_export(t_env *env);
char		*ft_getenv(t_env *env, char *str, size_t n);

/* ************************************************************************** */
// fd.c
void		fd_init(t_fds *fd);
void		fd_close(t_fds *fd);

#endif
