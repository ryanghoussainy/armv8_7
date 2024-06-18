#include "shell.h"
#include "instruction.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/rmdir.h"
#include "commands/rm.h"
#include "commands/mkdir.h"
#include "commands/touch.h"

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

void execute_instruction(Shell* shell, Instruction ins) {
    switch (ins.operation) {
        case LS:
            if (ins.argument_count > 0)
                ls(shell, ins.arguments[0]);
            else
                ls (shell, NULL);
            break;
        case CD:
            cd(shell, ins.arguments[0]);
            break;
        case TOUCH:
            touch(shell, ins.arguments[0]);
            break;
        case MKDIR:
            mkdir(shell, ins.arguments[0]);
            break;
        default:
            break;
    }
    printf("\n");
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

        Instruction ins = parse_to_instruction(line);
        execute_instruction(shell, ins);
    }

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
}

int main(int argc, char **argv) {
    Shell shell;
    initialise_shell(&shell, stdout);

    mkdir(&shell, "test1");
    mkdir(&shell, "test2");
    touch(&shell, "file1");

    mkdir(&shell, "test1/test3");
    mkdir(&shell, "test1/test3/../../test1/test3/test4");
    
    touch(&shell, "test1/test3/test4/file2");

    execute_file(&shell, "ins.txt");

    return 0;
}