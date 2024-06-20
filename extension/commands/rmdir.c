#include "rmdir.h"
#include "cd.h"

void rmdir(Shell* shell, char* dirpath)
{
    char* initial_path = strdup(shell->path);
    char* prefixed_dirpath = NULL;

    if (dirpath[0] != '/') {
        prefixed_dirpath = malloc(strlen("./") + strlen(dirpath) + 1);
        strcpy(prefixed_dirpath, "./");
        prefixed_dirpath = strcat(prefixed_dirpath, dirpath);
    }else {
        prefixed_dirpath = strdup(dirpath);
    }
    
    char* path = previous_directory_path(prefixed_dirpath);
    char* name = prefixed_dirpath + strlen(path);
    
    cd(shell, path);

    Directory* delete_this_dir = dir_find_directory(shell->current_directory, name);

    if (delete_this_dir == NULL) {
        printf("Directory %s does not exist\n", name);
        return;
    }

    dir_remove_directory(shell->current_directory, delete_this_dir);

    cd(shell, initial_path);
    free(initial_path);
    free(prefixed_dirpath);
    free(path);
}