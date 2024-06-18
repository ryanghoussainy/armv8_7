#include "shell.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/rmdir.h"
#include "commands/rm.h"
#include "commands/mkdir.h"
#include "commands/touch.h"
#include "commands/echo.h"

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
    touch(&shell, "file1");

    mkdir(&shell, "test1/test3");
    mkdir(&shell, "test1/test3/../../test1/test3/test4");

    touch(&shell, "test1/test3/test4/file2");


    rm(&shell, "file1");
    rmdir(&shell, "test2");

    ls(&shell, NULL);
    printf("\n");
    ls(&shell, "test1");
    printf("\n");
    ls(&shell, "test1/test3");
    printf("\n");
    ls(&shell, "test1/test3/test4");

    Directory* test1 = dir_find_directory(shell.root, "test1");
    Directory* test3 = dir_find_directory(test1, "test3");
    Directory* test4 = dir_find_directory(test3, "test4");
    File* file2 = dir_find_file(test4, "file2");

    printf("\n%s\n", file2->content);
    echo(&shell, "Hello, World!", "test1/test3/test4/file2", false);
    echo(&shell, "Hello, World!!!", "test1/test3/test4/file2", true);
    echo(&shell, "Hi there...", "test1/something", false);
    printf("\n%s\n", file2->content);

    File* something = dir_find_file(test1, "something");
    printf("\n%s\n", something->content);

    print_shell(&shell);

    return 0;
}