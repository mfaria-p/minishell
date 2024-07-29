#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef struct s_token
{
	int				code;
	char			*content;
	char			unexpected;
}					t_token;

typedef struct s_node_default
{
	int				node_type;
}					t_node_default;

typedef struct s_node_pipe
{
	int				node_type;
	t_node_default	*left_node;
	t_node_default	*right_node;
}					t_node_pipe;

typedef struct s_node_redirect
{
	int				node_type;
	char			*filename;
	char			*delimeter;
	t_node_default	*next;
}					t_node_redirect;

typedef struct s_node_execution
{
	int				node_type;
	char			*command;
	int				n_params;
	char			**params;
}					t_node_execution;

// Node types
enum				e_nodetype
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
	int in;
	int out;
}	t_fds;

t_node_default	*execution(struct s_node_default *node, t_env *env, pid_t pid, t_fds *fd, int *wstatus);
int				create_heredoc(const char *delimiter, const char *file_name);
char			*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);

t_token			lex(char *str, int *wstatus);
t_node_default	*parse(void);
void			destroy_tree(t_node_default *node);
int				siginit(void);

t_env	init_env(char ***export, char ***envp2, char **envp);
void			free_env_export(t_env *env);
//t_env	init_env_envp(char ***envp2, char **envp);
void			main_loop(t_env *env);

#endif
