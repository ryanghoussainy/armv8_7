#include "touch.h"
#include "cd.h"


/*
Creates a file at the provided path.
Path can be absolute or relative.
*/
void touch(Shell* shell, char* path) {
    if (strcmp(path, "/") == 0) {
        fprintf(shell->out, "Cannot create root directory\n");
        return;
    }

    char* last_slash = strrchr(path, '/');
    if (last_slash == NULL) { // For example, if the path is just a name: touch hello
        create_file(shell->current_directory, path); 
        return;
    }
    // The path includes directories: touch d1/d2/hello / touch /d1/d2/hello
    
    char* initial_path = strdup(shell->path);
    if (path[0] == '/') {
        // Absolute path: change directory to root
        cd(shell, "/");
    }

    char* file_name = last_slash + 1; // + 1 to skip the '/'

    size_t parent_path_len = last_slash - path;
    char* parent_path = malloc(parent_path_len + 1);  // +1 for the null terminator
    assert(parent_path != NULL);

    strncpy(parent_path, path, parent_path_len);
    parent_path[last_slash - path] = '\0';

    cd(shell, parent_path);  // Change directory to the parent directory
    create_file(shell->current_directory, file_name);
    cd(shell, initial_path);  // Go back to the original path

    free(parent_path);
    free(initial_path);
}
