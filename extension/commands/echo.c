#include "echo.h"
#include "touch.h"
#include "cd.h"


/*
Writes the string either to shell.out or to a file.
*/
void echo(Shell* shell, char* str, char* path, bool append) {
    if (path == NULL) {
        fprintf(shell->out, "%s\n", str);
        return;
    }

    if (strcmp(path, "/") == 0) {
        printf("Cannot write to root directory\n");
        exit(1);
    }

    // Write str to file
    File* write_file;
    if (strchr(path, '/') == NULL) {
        write_file = dir_find_file(shell->current_directory, path);
        if (write_file == NULL) {
            touch(shell, path); // create file if it doesn't exist
            write_file = dir_find_file(shell->current_directory, path);
            assert(write_file != NULL); // File should now exist
        }
    } else { 
        // Get name of file
        char* previous_path = previous_directory_path(path);
        char* file_name = path + strlen(previous_path) + 1; // +1 to skip the '/'

        if (dir_find_file(shell->current_directory, file_name) == NULL) {
            touch(shell, path); // create file if it doesn't exist
        }

        char* initial_path = shell->path;
        cd(shell, previous_path);
        
        write_file = dir_find_file(shell->current_directory, file_name);
        assert(write_file != NULL); // File should now exist

        cd(shell, initial_path);  // Go back to the original path
    }

    file_write(write_file, str, append);
}
