/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:39:26 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/08/02 16:57:06 by ecorona-         ###   ########.fr       */
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

// EXECUTION

t_node_d	*execution(t_node_d *node, t_sh sh);

// pipe executing functions
void		exec_pipe(t_node_p *pip, t_sh sh);
pid_t		have_child(t_node_p *pip, int rw, int pipefd[2], t_sh sh);

// redirect executing functions
void		exec_red(t_node_r *red, t_sh sh);
char		*find_home(char **envp);
void		exec_not_heredoc(t_node_r *red, int flags, int io, t_env *env);
void		exec_heredoc(t_node_r *red, t_env *env);
int			file_exist(const char *filename);

// Simple commands executing functions
void		exec_exec(t_node_e *exec, t_node_d *root, t_fds *fd, t_sh sh);
char		*find_path(char **envp);
char		*get_cmd(char **paths, char *cmd);
char		*find_the_command(char **envp, t_node_e *exec);
void		ft_execute(t_node_e *exec, char **envp, t_node_d *root);

// utils
size_t		ft_strlen(const char *s);
char		**ft_split(char const *s, char c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strjoin(char const *s1, char const *s2);
void		*ft_memcpy(void *dest, const void *src, size_t n);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s);
int			ft_strcmp(char *s1, char *s2);
int			ft_isdigit(int c);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);

// not done (built ins)
// dividir o main em dois pk ta mt grande
void		ft_echo(char **params);
int			ft_countchar(const char *str, char c);

void		ft_cd(t_env *env, char *path);
char		*find_var2(char *name);
char		*find_oldpwd(char **envp);
char		*create_env_var(const char *var, const char *value);
void		ft_cd_home(t_env *env);
void		update_pwd(t_env *env, const char *current);
void		update_oldpwd(t_env *env, const char *current);

void		ft_pwd(char **envp);

void		ft_printexport(char **export);
void		ft_doexport(t_env *env, char **params, int *wstatus);
char		**resize_and_add(char ***envp, char *new_var);
void		set_env_with_equal(char ***envp, char *var_value);
void		set_env_without_equal(char ***envp, char *var);
void		set_env_with_equal_plus(char ***envp, char *var_value);
int			is_valid_identifier(char *var, char *value);
int			is_valid_identifier_plus(char *var, char *value);
int			find_var(char **envp, const char *var);
int			compare_env_var(const char *env_var, const char *var, int len2);
int			get_var_length(const char *var);

void		ft_unset(char **args, t_env *env);
char		**remove_var(char **envp, char *var);
char		**allocate_env_array(char **envp);
int			matches_var(char *env_entry, char *var, int len2);
char		**remove_var(char **envp, char *var);

void		ft_free(char **envp);

void		ft_printenv(char **envp);

// Error_handling
int			ft_error(int error);
int			cmd_not_found(char *str);
void		ft_putstr_fd(char *s, int fd);
int			file_not_found(char *str);
void		error_identifier(char *var, char *value);
void		error_identifier_plus(char *var, char *value);
int			error_envp(char *str);
int			err_cd(void);

#endif
