#include "rmdir.h"
#include "cd.h"

void rm(Shell* shell, char* filepath)
{
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

    File* rm_file = dir_find_file(shell->current_directory, name);

    if (rm_file == NULL) {
        printf("File %s does not exist\n", name);
        return;
    }

    dir_remove_file(shell->current_directory, rm_file);

    cd(shell, initial_path);
}