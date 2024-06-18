#include "shell.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/rmdir.h"

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

int main(void) {
    Shell shell;
    initialise_shell(&shell, stdout);

    Directory* test1 = create_dir(shell.current_directory, "test1");
    create_dir(shell.current_directory, "test2");
    create_file(shell.current_directory, "file1");

    Directory* test3 = create_dir(test1, "test3");
    create_file(test1, "file2");

    create_file(test3, "file3");
    
    ls(&shell);
    rmdir(&shell, "test2");
    ls(&shell);

    return 0;
}