#include "rmdir.h"
#include "cd.h"

void rmdir(Shell* shell, char* dirpath)
{
    char* initial_path = strdup(shell->path);

    if (strcmp(dirpath, "/") == 0) {
        fprintf(shell->out, "Cannot delete root directory\n");
        return;
    }

    char* last_slash = strrchr(dirpath, '/');
    char* name;
    if (last_slash != NULL) {
        // The path includes directories: mkdir d1/d2/hello
        name = last_slash + 1; // + 1 to skip the '/'
        size_t parent_path_len = last_slash - dirpath;
        char* parent_path = malloc(parent_path_len + 1);  // +1 for the null terminator
        assert(parent_path != NULL);
        
        strncpy(parent_path, dirpath, parent_path_len);
        parent_path[last_slash - dirpath] = '\0';

        cd(shell, parent_path);
        
        free(parent_path);
    } else {
        name = dirpath;
    }

    Directory* rm_dir = dir_find_directory(shell->current_directory, name);

    if (rm_dir == NULL) {
        printf("Directory %s does not exist\n", name);
        return;
    }

    dir_remove_directory(shell->current_directory, rm_dir);

    cd(shell, initial_path);
    free(initial_path);
}