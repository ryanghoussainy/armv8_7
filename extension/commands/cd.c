#include "cd.h"

/*
Change directory to the given path.
Path can be absolute or relative.
*/
void cd(Shell* shell, char* path) {
    assert(path != NULL);

    if (strcmp(path, "/") == 0) {
        shell->current_directory = shell->root;
        free(shell->path);
        shell->path = strdup("/");
        assert(shell->path != NULL);
        return;
    }

    char* path_copy = strdup(path);
    assert(path_copy != NULL);

    char* token = strtok(path_copy, "/");

    if (path[0] == '/') { // Absolute path
        shell->current_directory = shell->root;
        free(shell->path);
        shell->path = strdup("/");
    }

    while (token != NULL) {  // loop through the directories in the path
        if (strcmp(token, "..") == 0) {
            if (shell->current_directory->parent != NULL) {
                shell->current_directory = shell->current_directory->parent;

                char* temp = previous_directory_path(shell->path);
                free(shell->path);
                shell->path = temp;
            } else {
                printf("Root does not have a parent directory.\n");
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
            char* temp = get_file_path(shell->path, token);
            free(shell->path);
            shell->path = temp;
        }
        token = strtok(NULL, "/");
    }
    free(path_copy);
}
