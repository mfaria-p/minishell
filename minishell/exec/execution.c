/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/26 15:25:35 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
//NO LOOP DO MINISHELL, antes de chamarmos a execucao temos qeu prepara la, aka voltar a por o stdin e o stdout normais
//falta error handling no here doc (mb tenho q cleanar o temp file dps?)
//o redirect input e append n ta a dar (da segmentation fault), nem o seu erro handling

// Function to replicate the echo command
void ft_echo(char **params) 
{
    int i = 0;
    int newline = 1;  // Flag to determine whether to print a newline at the end

    // Check for the -n flag
    if (params[0] != NULL && ft_strncmp(params[0], "-n", 3) == 0) 
    {
        newline = 0;
        i = 1;
    }
    // Print the arguments
    while (params[i] != NULL) 
    {
        if (i > 0) {
            printf(" ");
        }
        printf("%s", params[i]);
        i++;
    }
    // Print newline if -n flag is not set
    if (newline)
        printf("\n");
}

void ft_cd(char **envp, char **params) { /* Implementation for cd */ }
void ft_pwd() { /* Implementation for pwd */ }
void ft_printexport(char **envp) { /* Implementation for export */ }
void ft_doexport(char **envp, char **params) { /* Implementation for export */ }
void ft_unset(char **args) { /* Implementation for unset */ }
void ft_printenv(char **envp) { /* Implementation for env */ }
void ft_exit(char **args) { /* Implementation for exit */ }

void  have_child(struct s_node_pipe *pip , int  rw, int pipefd[2], char **envp)
{
	int	pid;

	pid = fork();
    if (pid < 0) 
        ft_error(1);
	if (pid == 0)
	{
		dup2(pipefd[rw], rw);
        if (dup2(pipefd[rw], rw) == -1) 
            ft_error(2);
		close(pipefd[0]);
		close(pipefd[1]);
        if (rw == PIPE_WRITE)
            execution((struct s_node_default *)pip->left_node, envp);
        else 
            execution((struct s_node_default *)pip->right_node, envp);
	}
}

void  exec_pipe(struct s_node_pipe *pip, char **envp)
{
	int	pipefd[2];
	int	pid;

	pipe(pipefd);
    if (pipe(pipefd) == -1)
        ft_error(2);
	have_child(pip, PIPE_WRITE, pipefd, envp);
	have_child(pip, PIPE_READ, pipefd, envp);
	waitpid(-1, NULL, 0);
}

void  exec_red(struct s_node_redirect *red, char **envp)
{
	if (red->node_type == R_out)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_TRUNC, STDOUT_FILENO, envp);
	else if (red->node_type == R_app)
		exec_not_heredoc(red, O_CREAT | O_WRONLY, STDOUT_FILENO, envp);
	else if (red->node_type == R_heredoc)
		exec_heredoc(red, envp);
	else
		exec_not_heredoc(red, O_RDONLY, STDIN_FILENO, envp);
    if (red->next)
    execution((struct s_node_default *) red->next, envp);
}

void    exec_exec(struct s_node_execution *exec, char **envp)
{
    if (exec->node_type == E_builtin)
    {
        if (!ft_strncmp(exec->command, "echo", 5))
            ft_echo(exec->params);
        else if (!ft_strncmp(exec->command, "cd", 3))
            ft_cd(envp, exec->params);
        else if (!ft_strncmp(exec->command, "pwd", 4))
            ft_pwd();
        else if (!ft_strncmp(exec->command, "export", 7) && !exec->params)
		    ft_printexport(envp);
	    else if (!ft_strncmp(exec->command, "export", 7) && exec->params[0])
		    ft_doexport(envp, exec->params);
        else if (!ft_strncmp(exec->command, "unset", 6))
            ft_unset(exec->params);
        else if (!ft_strncmp(exec->command, "env", 4))
            ft_printenv(envp);
        else if (!ft_strncmp(exec->command, "exit", 5))
            ft_exit(exec->params);
    }
    else
        ft_execute(exec, envp);
}
char *find_path(char **envp)
{
    while (*envp && ft_strncmp("PATH=", *envp, 5) != 0)
        envp++;
    if (*envp)
        return (*envp + 5);
    return NULL;
}

char *get_cmd(char **paths, char *cmd)
{
    char *tmp;
    char *command;

    while (*paths)
    {
        tmp = ft_strjoin(*paths, "/");
        command = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(command, 0) == 0)
            return (command);
        free(command);
        paths++;
    }
   cmd_not_found(cmd);
    return (NULL);
}

char    *find_the_command(char **envp, struct s_node_execution *exec)
{
    char    *paths;
    char    **cmd_paths;
    char*   command;
    int i;
    
    i = 0;
    paths = find_path(envp);
    cmd_paths = ft_split(paths, ':');
    command = get_cmd(cmd_paths, exec->command);
    while (cmd_paths[i])
    {
        free(cmd_paths[i]);
        i++;
    }
    free(cmd_paths);
    return (command);
}

void ft_execute(struct s_node_execution *exec, char **envp)
{
    char*   command;
    int param_count;
    char **argv;
    int i;
    
    i = 0;
    param_count = 0;
    command=find_the_command(envp, exec);
    while (exec->params && exec->params[param_count] != NULL) 
        param_count++;
    argv = (char **)malloc((param_count + 2) * sizeof(char *));
    argv[0] = command; // First argument is the command itself
    while (i < param_count)
    {
        argv[i + 1] = exec->params[i];
        i++;
    }
    argv[param_count + 1] = NULL;
    execve(command, argv, envp);
    free(command);
}

void exec_not_heredoc(struct s_node_redirect *red, int flags, int io, char **envp)
{
	int	fd;
	fd = open(red->filename, flags, MODE);
    if (fd == - 1 && io == STDIN_FILENO)
        ft_error(4);
    else if (fd == - 1 && io == STDOUT_FILENO)
        ft_error(3); 
	dup2(fd, io);
    if (dup2(fd, io) == -1)
    {
        close(fd);
       ft_error(3);
    } 
	close(fd);
    //execution((struct s_node_default *) red->next, envp);
    exit(0);
}

// Function to read input for the heredoc and write it to a temporary file
int create_heredoc(const char *delimiter, const char *file_name) {
    int fd;
    char *line;

    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    line = readline(HEREDOC_MSG);
    while (line && ft_strncmp(delimiter, line, ft_strlen(delimiter)) != 0) {
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
        line = readline(HEREDOC_MSG);
    }
    free(line);
    close(fd);
    if (line == NULL) 
        return EXIT_FAILURE;
    else 
        return EXIT_SUCCESS;
}

void have_child_hd(struct s_node_redirect *red, char **envp, const char *file_name) {
    int pid;
    int fd;

    pid = fork();
    if (pid == 0) {
        fd = open(file_name, O_RDONLY);
        if (fd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);  // Redirect stdin to the temporary file
        close(fd);
        execution((struct s_node_default *)red->next, envp);  // Execute the command
        exit(0);  // Exit the child process after execution
    }
}

void exec_heredoc(struct s_node_redirect *red, char **envp) {
    const char *temp_file_name = "/tmp/heredoc_tmp";

    if (create_heredoc(red->delimeter, temp_file_name) == EXIT_SUCCESS) {
        have_child_hd(red, envp, temp_file_name);
        waitpid(-1, NULL, 0);  // Wait for the child process to finish
        //exit(EXIT_SUCCESS);
        //TENHO QUE CLEANAR O TEMP FILE DPS
    } else {
        exit(EXIT_FAILURE);
    }
}

void  execution(struct s_node_default *node, char **envp)
{
	if ((node->node_type & E_cmd) || (node->node_type & E_builtin))
		exec_exec((struct s_node_execution *) node, envp);
	else if (node->node_type & (1 << 5))
		exec_red((struct s_node_redirect *) node, envp);
	else
		exec_pipe((struct s_node_pipe *) node, envp);
}

//Test

// Function to create a s_node_execution
struct s_node_execution *create_execution_node(char *command, char **params, int type) 
{
    struct s_node_execution *exec_node = malloc(sizeof(struct s_node_execution));
    if (!exec_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    exec_node->node_type = type;
    exec_node->command = command;
    exec_node->params = params;
    return exec_node;
}

// Function to create a s_node_redirect
struct s_node_redirect *create_redirect_node(int type, char *filename, char *delimiter, struct s_node_default *next) 
{
    struct s_node_redirect *red_node = malloc(sizeof(struct s_node_redirect));
    if (!red_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    red_node->node_type = type;
    red_node->filename = filename;
    red_node->delimeter = delimiter;
    red_node->next = next;
    return red_node;
}

// Function to create a s_node_pipe
struct s_node_pipe *create_pipe_node(struct s_node_default *left_node, struct s_node_default *right_node) 
{
    struct s_node_pipe *pipe_node = malloc(sizeof(struct s_node_pipe));
    pipe_node->node_type = P;
    pipe_node->left_node = left_node;
    pipe_node->right_node = right_node;
    return pipe_node;
}

void print_node_type(int code) {
    switch (code) {
        case E_cmd: printf("E_cmd "); break;
        case E_builtin: printf("E_builtin "); break;
        case R_out: printf("R_out "); break;
        case R_app: printf("R_app "); break;
        case R_heredoc: printf("R_heredoc "); break;
        case R_input: printf("R_input "); break;
        case P: printf("P "); break;
        default: printf("Unknown code: %d", code); break;
    }
    printf("\n");
}
