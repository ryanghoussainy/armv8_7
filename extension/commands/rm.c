#include "rmdir.h"

void rm(Shell* shell, char* name)
{
    File* rm_file = dir_find_file(shell->current_directory, name);

    if (rm_file == NULL) {
        printf("File %s does not exist\n", name);
        return;
    }

    dir_remove_file(shell->current_directory, rm_file);
}