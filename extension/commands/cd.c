#include "cd.h"

/*
Change directory to the given path.
Path can be absolute or relative.
*/
void cd(Shell* shell, char* path) {
    assert(path != NULL);

    if (strcmp(path, "/") == 0) {
        shell->current_directory = shell->root;
        shell->path = strdup("/");
        return;
    }

    char* token = strtok(path, "/");

    if (path[0] == '/') { // Absolute path
        shell->current_directory = shell->root;
        shell->path = strdup("/");
    } else {              // Relative path
        shell->current_directory = shell->current_directory;
        shell->path = strdup(shell->path);
    }

    while (token != NULL) {  // loop through the directories in the path
        if (strcmp(token, "..") == 0) {
            if (shell->current_directory->parent != NULL) {
                shell->current_directory = shell->current_directory->parent;
                shell->path = previous_directory_path(shell->path);
            } else {
                printf("Root does not have a parent directory: %s\n", path);
                exit(1);  // Crashes if user tries to go backwards from root
            }
        } else if (strcmp(token, ".") == 0) {
            // do nothing
        } else {
            shell->current_directory = dir_find_directory(shell->current_directory, token);
            if (shell->current_directory == NULL) { // Couldn't find the directory
                printf("Directory not found. Token: %s\n", token);
                exit(1);
            }
            shell->path = get_file_path(shell->path, token);
        }
        token = strtok(NULL, "/");
    }
}
