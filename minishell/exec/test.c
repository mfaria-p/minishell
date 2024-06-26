
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
