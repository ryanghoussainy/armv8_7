#include "shell.h"
#include "command.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/rmdir.h"
#include "commands/rm.h"
#include "commands/mkdir.h"
#include "commands/touch.h"
#include "commands/echo.h"
#include "commands/cat.h"
#include "commands/cp.h"
#include "commands/mv.h"

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

void execute_command(Shell* shell, Command* cmd) {
    switch (cmd->operation) {
        case LS:
            if (cmd->argument_count > 0)
                ls(shell, cmd->arguments[0]);
            else
                ls (shell, NULL);
            break;
        case CD:
            cd(shell, cmd->arguments[0]);
            break;
        case TOUCH:
            touch(shell, cmd->arguments[0]);
            break;
        case MKDIR:
            mkdir(shell, cmd->arguments[0]);
            break;
        case CAT:
            cat(shell, cmd->arguments[0]);
            break;
        case RM:
            rm(shell, cmd->arguments[0]);
            break;
        case RMDIR:
            rmdir(shell, cmd->arguments[0]);
            break;
        default:
            break;
    }
}

void execute_file(Shell* shell, const char *filename) {
    int MAX_LINE_LENGTH = 100;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
        }

        
        fprintf(shell->out, "\n(%s) %s:\n", shell->path, line);
        Command cmd = parse_to_command(line);
        execute_command(shell, &cmd);
    }

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
}

int main(int argc, char **argv) {
    char* FILE_IN;
    char* FILE_OUT;

    if (argc == 2) {
        FILE_IN = argv[1];
        FILE_OUT = "stdout";
    } else if (argc == 3) {
        FILE_IN = argv[1];
        FILE_OUT = argv[2];
    } else {
        printf("Wrong number of arguments supplied\n");
        return EXIT_FAILURE;
    }


    Shell shell;

    if (strcmp(FILE_OUT, "stdout") == 0)
        initialise_shell(&shell, stdout);
    else {
        FILE* out = fopen(FILE_OUT, "w");
        initialise_shell(&shell, out);
    }
    initialise_shell(&shell, stdout);

    mkdir(&shell, "test1");
    mkdir(&shell, "test2");
    touch(&shell, "file1");

    mkdir(&shell, "test1/test3");
    mkdir(&shell, "test1/test3/test4");

    touch(&shell, "test1/test3/test4/file2");

    execute_file(&shell, FILE_IN);

    // rm(&shell, "file1");
    // rmdir(&shell, "test2");

    ls(&shell, NULL);
    printf("\n");
    ls(&shell, "test1");
    printf("\n");
    ls(&shell, "test1/test3");
    printf("\n");
    ls(&shell, "test1/test3/test4");
    printf("\n");

    Directory* test1 = dir_find_directory(shell.root, "test1");
    Directory* test3 = dir_find_directory(test1, "test3");
    Directory* test4 = dir_find_directory(test3, "test4");
    File* file2 = dir_find_file(test4, "file2");

    printf("\n%s\n", file2->content);
    echo(&shell, "Hello, World!", "test1/test3/test4/file2", false);
    echo(&shell, "Hello, World!!!", "test1/test3/test4/file2", true);
    echo(&shell, "Hi there.", "test1/something", true);
    printf("\nfile2: %s\n", file2->content);

    File* something = dir_find_file(test1, "something");
    printf("\n%s\n", something->content);

    print_shell(&shell);

    cat(&shell, "test1/test3/test4/file2");
    return 0;
}