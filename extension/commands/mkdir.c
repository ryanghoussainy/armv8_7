#include "mkdir.h"
#include "cd.h"

/*
Creates a directory in the current directory or in the specified path.
*/
void mkdir(Shell* shell, char* path) {
    if (strcmp(path, "/") == 0) {
        fprintf(shell->out, "Cannot create root directory\n");
        return;
    }

    char* last_slash = strrchr(path, '/');
    if (last_slash == NULL) { // For example, if the path is just a name: mkdir hello
        create_dir(shell->current_directory, path); 
        return;
    }
    // The path includes directories: mkdir d1/d2/hello
    char* dir_name = last_slash + 1; // + 1 to skip the '/'

    size_t parent_path_len = last_slash - path;
    char* parent_path = malloc(parent_path_len + 1);  // +1 for the null terminator
    assert(parent_path != NULL);

    strncpy(parent_path, path, parent_path_len);
    parent_path[last_slash - path] = '\0';

    char* initial_path = strdup(shell->path);
    cd(shell, parent_path);
    create_dir(shell->current_directory, dir_name);
    cd(shell, initial_path);  // Go back to the original path
    free(initial_path);
    free(parent_path);
}
