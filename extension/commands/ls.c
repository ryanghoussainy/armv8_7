#include "ls.h"

void ls(Shell* shell)
{
    Node* curr_file = shell->current_directory->files->head;
    Node* curr_dir = shell->current_directory->directories->head;
    
    while (curr_file != NULL) {
        File* file = (File*) curr_file->elem;
        fprintf(shell->out, "%s\n", file->name);
        curr_file = curr_file->next;
    }

    while (curr_dir != NULL) {
        Directory* dir = (Directory*) curr_dir->elem;
        fprintf(shell->out, "/%s\n", dir->name);
        curr_dir = curr_dir->next;
    }
}