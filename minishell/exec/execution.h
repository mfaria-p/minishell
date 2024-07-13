/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:39:26 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/07/13 12:31:33 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
//#include <string.h>
//#include <stdio.h>

# define PIPE_READ STDIN_FILENO
# define PIPE_WRITE STDOUT_FILENO
# define MODE 00644
# define HEREDOC_MSG "> "

/*
//Nodes structures
struct						s_node_default
{
	int	s_node_type; //PIPE REDIRECT EXEC
};

struct						s_node_pipe
{
	int						s_node_type;
	struct s_node_default	*left_node;
	struct s_node_default	*right_node;
};

struct						s_node_redirect
{
	int						s_node_type;
	char					*filename;
	char					*delimeter;
	struct s_node_default	*next;
};

struct						s_node_execution
{
	int						s_node_type;
	char					*command;
	char					**params;
};

//Node types
enum						e_nodetype
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

// EXECUTION

t_node_default				*execution(struct s_node_default *node, t_env *env, pid_t pid);

// pipe executing functions
void						exec_pipe(struct s_node_pipe *pip, t_env *env);
pid_t						have_child(struct s_node_pipe *pip, int rw,
								int pipefd[2], t_env *env);

// redirect executing functions
void						exec_red(struct s_node_redirect *red, t_env *env, pid_t pid);
void						exec_not_heredoc(struct s_node_redirect *red,
								int flags, int io, t_env *env);
void						exec_heredoc(struct s_node_redirect *red,
								t_env *env);
/* void						have_child_hd(struct s_node_redirect *red,
								t_env *env, const char *file_name); */
int							create_heredoc(const char *delimiter,
								const char *file_name);
int							file_exist(const char *filename);
/* void						have_child_n_hd(struct s_node_redirect *red,
								t_env *env, int flags, int io); */

// Simple commands executing functions
void						exec_exec(struct s_node_execution *exec,
								t_env *env, pid_t pid);
char						*find_path(char **envp);
char						*get_cmd(char **paths, char *cmd);
char						*find_the_command(char **envp,
								struct s_node_execution *exec);
void						ft_execute(struct s_node_execution *exec,
								char **envp);

// utils
size_t						ft_strlen(const char *s);
char						**ft_split(char const *s, char c);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
char						*ft_strjoin(char const *s1, char const *s2);
void						*ft_memcpy(void *dest, const void *src, size_t n);
int							ft_isalnum(int c);
int							ft_isalpha(int c);
char						*ft_strchr(const char *s, int c);
char						*ft_strdup(const char *s);
int							ft_strcmp(char *s1, char *s2);
int							ft_isdigit(int c);
size_t						ft_strlcpy(char *dst, const char *src, size_t size);

// not done (built ins)
// dividir o main em dois pk ta mt grande
void						ft_echo(char **params);
int							ft_countchar(const char *str, char c);

void	ft_cd(t_env *env, char *path);
char	*find_var2(char	*name);
char						*create_env_var(const char *var, const char *value);
void						ft_cd_home(t_env	*env);

void						ft_pwd(char **envp);

void						ft_printexport(char **export);
void						ft_doexport(t_env *env, char **params);
char						**resize_and_add(char ***envp, char *new_var);
void						set_env_with_equal(char ***envp, char *var_value);
void						set_env_without_equal(char ***envp, char *var);
void						ft_doexport(t_env *env, char **params);
int							is_valid_identifier(char *var, char *value);
void	set_env_with_equal_envp(char ***envp, char *var_value);
// void						sort_env(char **envp);
int							find_var(char **envp, const char *var);
char	**resize_and_add_envp(char ***envp, char *new_var);
char	**remove_var(char **envp, char *var);
char	**remove_var_envp(char **envp, char *var);
void ft_free(char **envp);

void	ft_unset(char **args, t_env *env);

void						ft_printenv(char **envp);

// Error_handling
int							ft_error(int error);
int							cmd_not_found(char *str);
void						ft_putstr_fd(char *s, int fd);
int							file_not_found(char *str);
void						error_identifier(char *var, char *value);
int	error_envp(char *str);

#endif
