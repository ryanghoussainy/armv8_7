#include "echo.h"
#include "touch.h"
#include "cd.h"


/*
Writes the string either to shell.out or to a file.
*/
void echo(Shell* shell, char* str, bool redirect, char* redirect_path, bool append) {
    if (!redirect) {
        fprintf(shell->out, "%s\n", str);
        return;
    }

    // Redirect to file
    if (strcmp(redirect_path, "/") == 0) {
        printf("Cannot write to root directory\n");
        return;
    }

    // Write str to file
    File* write_file;
    if (strchr(redirect_path, '/') == NULL) {
        write_file = dir_find_file(shell->current_directory, redirect_path);
        if (write_file == NULL) {
            touch(shell, redirect_path); // create file if it doesn't exist
            write_file = dir_find_file(shell->current_directory, redirect_path);
            assert(write_file != NULL); // File should now exist
        }
    } else { 
        // Get name of file
        char* previous_path = previous_directory_path(redirect_path);
        char* file_name = redirect_path + strlen(previous_path) + 1; // +1 to skip the '/'

        if (dir_find_file(shell->current_directory, file_name) == NULL) {
            touch(shell, redirect_path); // create file if it doesn't exist
        }

        char* initial_path = shell->path;
        cd(shell, previous_path);
        
        write_file = dir_find_file(shell->current_directory, file_name);
        assert(write_file != NULL); // File should now exist

        cd(shell, initial_path);  // Go back to the original path
    }

    file_write(write_file, str, append);
}
