/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 12:38:37 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/22 19:13:59 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
            ft_printf(" ");
        }
        ft_printf("%s", params[i]);
        i++;
    }
    // Print newline if -n flag is not set
    if (newline)
        ft_printf("\n");
}

void ft_cd(char **envp, char **params) { /* Implementation for cd */ }
void ft_pwd() { /* Implementation for pwd */ }
void ft_printexport(char **envp) { /* Implementation for export */ }
void ft_doexport(char **envp, char **params) { /* Implementation for export */ }
void ft_unset(char **args) { /* Implementation for unset */ }
void ft_printenv(char **envp) { /* Implementation for env */ }
void ft_exit(char **args) { /* Implementation for exit */ }

void  have_child(struct node_pipe *pip , int  rw, int pipefd[2], char **envp)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(pipefd[rw], rw);
		close(pipefd[0]);
		close(pipefd[1]);
        if (rw == PIPE_WRITE)
            execution((struct node_default *)pip->left_node, envp);
        else 
            execution((struct node_default *)pip->right_node, envp);
	}
}

void  exec_pipe(struct node_pipe *pip, char **envp)
{
	int	pipefd[2];
	int	pid;

	pipe(pipefd);
	have_child(pip, PIPE_WRITE, pipefd, envp);
	have_child(pip, PIPE_READ, pipefd, envp);
	waitpid(0, NULL, 0);
}

void  exec_red(struct node_redirect *red, char **envp)
{
	if (red->node_type == R_out)
		exec_not_heredoc(red, O_CREAT | O_WRONLY | O_TRUNC, STDOUT_FILENO);
	else if (red->node_type == R_app)
		exec_not_heredoc(red, O_CREAT | O_WRONLY, STDOUT_FILENO);
	else if (red->node_type == R_heredoc)
		exec_heredoc(red);
	else
		exec_not_heredoc(red, O_RDONLY, STDIN_FILENO);
    if (red->next)
    execution((struct node_default *) red->next, envp);
}

void    exec_exec(struct node_execution *exec, char **envp)
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
    return (NULL);
}

char    *find_the_command(char **envp, struct node_execution *exec)
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

void ft_execute(struct node_execution *exec, char **envp)
{
    char*   command;
    int param_count;
    char **argv;
    int i;
    
    i = 0;
    param_count = 0;
    command=find_the_command(envp, exec);
    while (exec->params[param_count] != NULL) 
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

void exec_not_heredoc(struct node_redirect *red, int flags, int io)
{
	int	fd;
	fd = open(red->filename, flags, MODE);
	dup2(fd, io);
	close(fd);
}

void  exec_heredoc(struct node_redirect *red)
{
	/* char *line = 0;
    //mudar isto para ft_strcmp
	while (ft_strncmp(line, red->delimeter, 0))
	{
		line = readline("ʕ•́ᴥ•̀ʔ\n>");
		// duvidas
	} */
}

void  execution(struct node_default *node, char **envp)
{
	if ((node->node_type & E_cmd) || (node->node_type & E_builtin))
		exec_exec((struct node_execution *) node, envp);
	else if (node->node_type & R_out 
        || node->node_type & R_app || node->node_type & R_heredoc 
        || node->node_type & R_input)
		exec_red((struct node_redirect *) node, envp);
	else
		exec_pipe((struct node_pipe *) node, envp);
}

//Test

// Function to create a node_execution
struct node_execution *create_execution_node(char *command, char **params, int type) 
{
    struct node_execution *exec_node = malloc(sizeof(struct node_execution));
    exec_node->node_type = type;
    exec_node->command = command;
    exec_node->params = params;
    return exec_node;
}

// Function to create a node_redirect
struct node_redirect *create_redirect_node(int type, char *filename, char *delimiter, struct node_default *next) 
{
    struct node_redirect *red_node = malloc(sizeof(struct node_redirect));
    red_node->node_type = type;
    red_node->filename = filename;
    red_node->delimeter = delimiter;
    red_node->next = next;
    return red_node;
}

// Function to create a node_pipe
struct node_pipe *create_pipe_node(struct node_default *left_node, struct node_default *right_node) 
{
    struct node_pipe *pipe_node = malloc(sizeof(struct node_pipe));
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

int main(int argc, char **argv, char **envp)
{
	struct node_default *root;
    
	// Test case: ls | grep ".h"
    char *ls_params[] = {NULL};
    struct node_execution *ls_node = create_execution_node("ls", ls_params, E_cmd);

    printf("Node type: ");
    print_node_type(ls_node->node_type);
    printf("Node1 content: %s\n", ls_node->command);
    printf("Node1 params: ");
    for (int i = 0; ls_node->params[i] != NULL; i++) {
        printf("%s ", ls_node->params[i]);
    }
    printf("\n");

    char *grep_params[] = {".h", NULL};
    struct node_execution *grep_node = create_execution_node("grep", grep_params, E_cmd);
    
    printf("Node type: ");
    print_node_type(grep_node->node_type);
    printf("Node2 content: %s\n", grep_node->command);
    printf("Node2 params: ");
    for (int i = 0; grep_node->params[i] != NULL; i++) {
        printf("%s ", grep_node->params[i]);
    }
    printf("\n");

    struct node_pipe *pipe_node = create_pipe_node((struct node_default *)ls_node, (struct node_default *)grep_node);
    
    root = (struct node_default *)pipe_node;
    printf("Executing: ls | grep \".h\"\n");
	execution(root, envp);
    
    // Test case: echo "Hello World" > output.txt
    char *echo_params[] = {"Hello World", NULL};
    struct node_execution *echo_node = create_execution_node("echo", echo_params, E_builtin);

    struct node_redirect *redirect_node = create_redirect_node(R_out, "output.txt", NULL, (struct node_default *)echo_node);

    root = (struct node_default *)redirect_node;
    printf("Executing: echo \"Hello World\" > output.txt\n");
    execution(root, envp);
    
	return EXIT_SUCCESS;
} 