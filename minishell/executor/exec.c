/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaria-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 22:59:10 by mfaria-p          #+#    #+#             */
/*   Updated: 2024/06/21 12:57:03 by mfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void ft_cd(char *path) { /* Implementation for cd */ }
void ft_pwd() { /* Implementation for pwd */ }
void ft_printexport() { /* Implementation for export */ }
void ft_doexport(char **args) { /* Implementation for export */ }
void ft_unset(char **args) { /* Implementation for unset */ }
void ft_printenv() { /* Implementation for env */ }
void ft_exit(char **args) { /* Implementation for exit */ }

void	exec_cmdline(t_node *tree, char **envp)
{
	if (tree == NULL)
        return;
    if (tree->lnode != NULL && tree->rnode == NULL) //n pode ser um pipe
        //sequencial, aka executa o left recursivamente
        //executa o primeiro comando e assim sucessivamente
        {
            exec_cmd(tree, envp);
            exec_cmdline(tree->lnode, envp);
        }
    else if (tree->lnode != NULL && tree->rnode != NULL) //vai ser um pipe
        //background o left
        //executa o right
        exec_pipeline(tree, envp);
    else
        exec_cmd(tree, envp);
}

void    exec_cmd(t_node *tree, char **envp)
{
    
    switch (tree->type)
    {
    case E_cmd:
        ft_execute(tree, envp);
        break;
    case E_builtin:
        exec_simple_cmd(tree, envp);
        break;
    case R_out:
    case R_app:
    case R_heredoc:
    case R_input:
        printf("Redirect node\n");
        // Handle redirection nodes
        break;
    default:
        printf("Unknown node type: %d\n", tree->type);
        break;
    }
}

void exec_pipeline(t_node *tree, char **envp)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process (left node)
        close(pipefd[0]);   // Close unused read end
        dup2(pipefd[1], 1); // Redirect stdout to pipe write end
        close(pipefd[1]);

        exec_cmdline(tree->lnode, envp);
        exit(0);
    }
    else
    {
        // Parent process (right node)
        close(pipefd[1]);   // Close unused write end
        dup2(pipefd[0], 0); // Redirect stdin to pipe read end
        close(pipefd[0]);

        exec_cmdline(tree->rnode, envp);
        waitpid(pid, NULL, 0);
    }
}

void ft_echo(char **params)
{
    printf("it is working, echo\n");
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

void ft_execute(t_node *tree, char **envp)
{
    char*   command;
    char    *paths;
    char    **cmd_paths;
    
    paths = find_path(envp);
    cmd_paths = ft_split(paths, ':');
    command=get_cmd(cmd_paths, tree->content);
    printf(command, "%s\n");
    //aqui ta algo sus:
    execve(command, tree->params, envp);
}

int ft_isbuiltin(t_node *cmd)
{
    if (!cmd->params[0])
        return 0;
    if (!ft_strncmp(cmd->content, "echo", 5) ||
        !ft_strncmp(cmd->content, "exit", 5) ||
        !ft_strncmp(cmd->content, "cd", 3) ||
        !ft_strncmp(cmd->content, "pwd", 4) ||
        !ft_strncmp(cmd->content, "export", 7) ||
        !ft_strncmp(cmd->content, "unset", 6) ||
        !ft_strncmp(cmd->content, "env", 4))
    {
        return 1;
    }
    return 0;
}

void exec_simple_cmd(t_node *tree, char **envp)
{
	if (!tree->content[0])
		return ;
    if (ft_isbuiltin(tree))
    {
        if (!ft_strncmp(tree->content, "echo", 5))
            ft_echo(tree->params);
        else if (!ft_strncmp(tree->content, "cd", 3))
            ft_cd(tree->params[1]);
        else if (!ft_strncmp(tree->content, "pwd", 4))
            ft_pwd();
        else if (!ft_strncmp(tree->content, "export", 7))
            ft_doexport(tree->params);
        else if (!ft_strncmp(tree->content, "unset", 6))
            ft_unset(tree->params);
        else if (!ft_strncmp(tree->content, "env", 4))
            ft_printenv();
        else if (!ft_strncmp(tree->content, "exit", 5))
            ft_exit(tree->params);
    }
    else
    {
        ft_execute(tree, envp);
    }
}

/////////////////////////////////////////////////////////////////////
//To test the codee
t_node* create_node(int code, char *content, char **params) {
    t_node *new_node = (t_node *)malloc(sizeof(t_node));
    new_node->type = code;
    new_node->content = strdup(content);
    new_node->params = params;
    new_node->lnode = NULL;
    new_node->rnode = NULL;
    return new_node;
}

// Function to print node type
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

int main(int argc, char **argv, char **envp) {
    // Example params
    char *params1[] = {"boop.txt", NULL};
    char *params2[] = {"-n", NULL};
    char    **paths;
    char    **cmd_paths;

    // Create example nodes
    t_node *node1 = create_node(E_cmd, "cat", params1);
    t_node *node2 = create_node(E_builtin, "echo", params2);
    
    // Print node types
    printf("Node1 type: ");
    print_node_type(node1->type);
    printf("Node1 content: %s\n", node1->content);
    printf("Node1 params: ");
    for (int i = 0; node1->params[i] != NULL; i++) {
        printf("%s ", node1->params[i]);
    }
    printf("\n");

    printf("Node2 type: ");
    print_node_type(node2->type);
    printf("Node2 content: %s\n", node2->content);
    printf("Node2 params: ");
    for (int i = 0; node2->params[i] != NULL; i++) {
        printf("%s ", node2->params[i]);
    }
    printf("\n");

    t_node *pipeline = create_node(P, "", NULL);
    pipeline->lnode = node1;
    pipeline->rnode = node2;

    // Execute command line
    exec_cmdline(node1, envp);

    // Free allocated memory
    free(node1->content);
    free(node1);
    free(node2->content);
    free(node2);
    free(pipeline);
    
    return 0;
}
