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
#include "commands/man.h"

void initialise_shell(Shell *shell, FILE *out)
{
    Directory *root = malloc(sizeof(Directory));
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

void print_shell(Shell *shell)
{
    printf("\nRoot: %s\n", shell->root->name);
    printf("Current directory: %s\n", shell->current_directory->name);
    printf("Path: %s\n\n", shell->path);
}

void execute_command(Shell *shell, Command *cmd)
{
    bool redirect;
    bool append;
    switch (cmd->operation) {
    case LS:
    case PWD:
    case CAT:
    case ECHO:
    case MAN:
        if (cmd->argument_count < 2) break; // Definitely no redirect

        if (strcmp(cmd->arguments[cmd->argument_count - 2], ">") == 0) {
            redirect = true;
            append = false;
            break;
        } else if (strcmp(cmd->arguments[cmd->argument_count - 2], ">>") == 0) {
            redirect = true;
            append = true;
            break;
        }
    default:
        redirect = false;
        append = false;
    }

    char** srcs;
    switch (cmd->operation) {
    case LS:
        if (cmd->argument_count > 0)
            ls(shell, cmd->arguments[0], redirect, cmd->arguments[cmd->argument_count - 1], append);
        else if (cmd->argument_count == 0)
            ls(shell, NULL, redirect, NULL, append);
        else
            ls(shell, NULL, redirect, cmd->arguments[cmd->argument_count - 1], append);
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
        cat(shell, cmd->arguments[0], redirect, cmd->arguments[cmd->argument_count - 1], append);
        break;
    case RM:
        rm(shell, cmd->arguments[0]);
        break;
    case RMDIR:
        rmdir(shell, cmd->arguments[0]);
        break;
    case CP: ;
        srcs = malloc(sizeof(char*) * (cmd->argument_count - 1));
        for (int i = 0; i < cmd->argument_count - 1; i++) {
            srcs[i] = cmd->arguments[i];
        }
        cp(shell, srcs, cmd->argument_count - 1, cmd->arguments[cmd->argument_count - 1]);
        break;
    case MV: ;
        srcs = malloc(sizeof(char*) * (cmd->argument_count - 1));
        for (int i = 0; i < cmd->argument_count - 1; i++) {
            srcs[i] = cmd->arguments[i];
        }
        mv(shell, srcs, cmd->argument_count - 1, cmd->arguments[cmd->argument_count - 1]);
        break;
    case ECHO:
        echo(shell, cmd->arguments[0], redirect, cmd->arguments[cmd->argument_count - 1], append);
        break;
    case PWD:
        pwd(shell, redirect, cmd->arguments[cmd->argument_count - 1], append);
        break;
    case MAN:
        man(shell, cmd, redirect, cmd->arguments[cmd->argument_count - 1], append);
    default:
        break;
    }
}

void execute_file(Shell *shell, const char *filename)
{
    int MAX_LINE_LENGTH = 100;
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        int line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n')
        {
            line[line_len - 1] = '\0';
        }

        fprintf(shell->out, "\n(%s) %s:\n", shell->path, line);
        Command cmd = parse_to_command(line);
        execute_command(shell, &cmd);
    }

    if (ferror(file))
    {
        perror("Error reading file");
    }

    fclose(file);
}

int main(int argc, char **argv)
{
    char *FILE_IN;
    char *FILE_OUT;

    if (argc == 2)
    {
        FILE_IN = argv[1];
        FILE_OUT = "stdout";
    }
    else if (argc == 3)
    {
        FILE_IN = argv[1];
        FILE_OUT = argv[2];
    }
    else
    {
        printf("Wrong number of arguments supplied\n");
        return EXIT_FAILURE;
    }

    Shell shell;

    if (strcmp(FILE_OUT, "stdout") == 0)
        initialise_shell(&shell, stdout);
    else
    {
        FILE *out = fopen(FILE_OUT, "w");
        initialise_shell(&shell, out);
    }

    execute_file(&shell, FILE_IN);

    return 0;
}