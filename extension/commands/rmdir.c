#include "rmdir.h"

void rmdir(Shell* shell, char* name)
{
    Directory* delete_this_dir = dir_find_directory(shell->current_directory, name);

    if (delete_this_dir == NULL) {
        printf("Direction %s does not exist", name);
        return;
    }

    dir_remove_directory(shell->current_directory, delete_this_dir);
}