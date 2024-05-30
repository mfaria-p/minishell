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

#endif
