#include "ls.h"
#include "cd.h"
#include "echo.h"

/*
Displays the contents of the current directory.
If a path is provided (i.e. is not NULL), it specifies the contents of
the directory at that path.
*/
void ls(Shell* shell, char* path, bool redirect, char* redirect_path, bool append)
{
    char* initial_path = shell->path;
    if (path != NULL) {
        cd(shell, path);   // Change directory to the path
    }

    Node* curr_file = shell->current_directory->files->head;
    Node* curr_dir = shell->current_directory->directories->head;
    
    if (redirect) {
        while (curr_file != NULL) {
            File* file = (File*) curr_file->elem;
            echo(shell, file->name, redirect, redirect_path, append);
            curr_file = curr_file->next;
        }
        while (curr_dir != NULL) {
            Directory* dir = (Directory*) curr_dir->elem;
            echo(shell, dir->name, redirect, redirect_path, append);
            curr_dir = curr_dir->next;
        }
    } else {
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

    if (path != NULL) {
        cd(shell, initial_path);  // Change directory back to the initial path
    }
}