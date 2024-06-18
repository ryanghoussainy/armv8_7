#include "shell.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/rmdir.h"
#include "commands/rm.h"
#include "commands/mkdir.h"

void initialise_shell(Shell* shell, FILE* out) {
    Directory* root = malloc(sizeof(Directory));
    root->name = strdup("/");
    root->files = create_linked_list((FreeFunc)free_file);
    root->directories = create_linked_list((FreeFunc)free_dir);
    root->path = strdup("/");
    root->parent = NULL;

    shell->root = root;
    shell->current_directory = root;
    shell->path = strdup("/");
    shell->out = out;
}

void print_shell(Shell* shell) {
    printf("\nRoot: %s\n", shell->root->name);
    printf("Current directory: %s\n", shell->current_directory->name);
    printf("Path: %s\n\n", shell->path);
}

int main(void) {
    Shell shell;
    initialise_shell(&shell, stdout);

    mkdir(&shell, "test1");
    mkdir(&shell, "test2");
    create_file(shell.current_directory, "file1");

    mkdir(&shell, "test1/test3");
    mkdir(&shell, "test1/test3/../../test1/test3/test4");
    
    // ls(&shell, NULL);

    // cd(&shell, "test1");

    ls(&shell, NULL);
    printf("\n");
    ls(&shell, "test1");
    printf("\n");
    ls(&shell, "test1/test3");

    print_shell(&shell);

    return 0;
}