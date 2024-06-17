#include "cd.h"

/*
Change directory to the given path.
Path can be absolute or relative.
*/
void cd(Shell* shell, char* path) {
    Directory* new_directory = find_directory(shell->root, path);
    if (new_directory == NULL) {
        fprintf(shell->out, "Directory not found: %s\n", path);
        return;
    }
    shell->current_directory = new_directory;
    fprintf(shell->out, "Current directory: %s\n", shell->current_directory->name);
}
