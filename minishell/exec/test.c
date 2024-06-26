/* 
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
} */

/*int main(int argc, char **argv, char **envp)*/
/*{*/
/*	struct s_node_default *root;*/
    
/* 	// Test case: ls | grep ".h"
    char *ls_params[] = {NULL};
    struct s_node_execution *ls_node = create_execution_node("ls", ls_params, E_cmd);

    printf("Node type: ");
    print_node_type(ls_node->node_type);
    printf("Node1 content: %s\n", ls_node->command);
    printf("Node1 params: ");
    for (int i = 0; ls_node->params[i] != NULL; i++) {
        printf("%s ", ls_node->params[i]);
    }
    printf("\n");

    char *grep_params[] = {".h", NULL};
    struct s_node_execution *grep_node = create_execution_node("grep", grep_params, E_cmd);
    
    printf("Node type: ");
    print_node_type(grep_node->node_type);
    printf("Node2 content: %s\n", grep_node->command);
    printf("Node2 params: ");
    for (int i = 0; grep_node->params[i] != NULL; i++) {
        printf("%s ", grep_node->params[i]);
    }
    printf("\n");

    struct s_node_pipe *pipe_node = create_pipe_node((struct s_node_default *)ls_node, (struct s_node_default *)grep_node);
    
    root = (struct s_node_default *)pipe_node;
    printf("Executing: ls | grep \".h\"\n");
	execution(root, envp); */
    
    // Test case: echo "Hello World" > output.txt
    /* char *echo_params[] = {"Hello World", NULL};
    struct s_node_execution *echo_node = create_execution_node("echo", echo_params, E_builtin);

    struct s_node_redirect *redirect_node = create_redirect_node(R_out, "output.txt", NULL, (struct s_node_default *)echo_node);

    root = (struct s_node_default *)redirect_node;
    printf("Executing: echo \"Hello World\" > output.txt\n");
    execution(root, envp); */
    
    // Test case: cat < input.txt
    /* char *cat_params[] = {NULL};
    struct s_node_execution *cat_node = create_execution_node("cat", cat_params, E_cmd);

    struct s_node_redirect *redirect_node3 = create_redirect_node(R_input, "input.txt", NULL, (struct s_node_default *)cat_node);

    root = (struct s_node_default *)redirect_node3;
    printf("Executing: cat < input.txt\n");
    execution(root, envp); */

    // Test case: echo cat <<EOF
    /* char *cat_params[] = {NULL};
    struct s_node_execution *cat_node = create_execution_node("cat", cat_params, E_cmd);

    struct s_node_redirect *redirect_node2 = create_redirect_node(R_heredoc, NULL, "EOF", (struct s_node_default *)cat_node);

    root = (struct s_node_default *)redirect_node2;
    printf("Executing: cat << EOF\n");
    execution(root, envp); */

    // Test case: echo 'Hello, World!' >> append.txt
/*    char *echo_params[] = {"Hello, World!", NULL};*/
/*    struct s_node_execution *echo_node = create_execution_node("echo", echo_params, E_cmd);*/
/**/
/*    struct s_node_redirect *redirect_node = create_redirect_node(R_app, "append.txt", NULL, (struct s_node_default *)echo_node);*/
/**/
/*    root = (struct s_node_default *)redirect_node;*/
/*    printf("Executing: echo 'Hello, World!' >> append.txt\n");*/
/*    execution(root, envp);*/
/**/
/*	return EXIT_SUCCESS;*/
/*} */
