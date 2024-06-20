#include "cat.h"
#include "cd.h"
#include "echo.h"


/*
'cat' prints the contents of a file

Future development will see more options and more capabilities
*/
void cat(Shell* shell, char* filepath, bool redirect, char* redirect_path, bool append) {

    // vv Using same code as in rm.c --> make a function?

    char* initial_path = shell->path;
    char* prefixed_filepath = NULL;

    if (filepath[0] != '/') {
        prefixed_filepath = malloc(strlen("./") + strlen(filepath) + 1);
        strcpy(prefixed_filepath, "./");
        prefixed_filepath = strcat(prefixed_filepath, filepath);
    }else {
        prefixed_filepath = strdup(filepath);
    }
    
    char* path = previous_directory_path(prefixed_filepath);
    char* name = prefixed_filepath + strlen(path) + 1;
    
    cd(shell, path);

    File* cat_file = dir_find_file(shell->current_directory, name);

    cd(shell, initial_path);

    if (cat_file != NULL) {
        if (redirect) {
            echo(shell, cat_file->content, true, redirect_path, append);
        } else {
            fwrite(cat_file->content, 1, strlen(cat_file->content) + 1, shell->out);
        }
    } else {
        char message[25] = "Empty or Invalid File";
        if (redirect) {
            echo(shell, message, true, redirect_path, append);
        } else {
            fwrite(message, 1, 25, shell->out);
        }
    }

    free(prefixed_filepath);
}