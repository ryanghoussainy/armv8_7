#include "cp.h"
#include "cd.h"
#include "rm.h"
#include "rmdir.h"

/*
Copies the contents of the files/dirs in srcs to the dest directory.
srcs is an array of file/dirs paths. src_count is the number of files/dirs in srcs.

Example: cp file1 dir1 file2 dest
*/

void cp(Shell* shell, char** srcs, int src_count, char* dest) {
    cp_mv(shell, srcs, src_count, dest, false);
}

void cp_mv(Shell* shell, char** srcs, int src_count, char* dest, bool is_mv) {
    assert(src_count > 0); // Should be checked before running this function

    for (int i = 0; i < src_count; i++) { // Loop through each source
        char* src_path = srcs[i];
        char* initial_path = strdup(shell->path);

        char* last_slash = strrchr(src_path, '/');
        if (last_slash != NULL) {
            // Go to the directory of the source file/dir
            size_t parent_path_len = last_slash - src_path;
            char* parent_path = malloc(parent_path_len + 1);  // +1 for the null terminator
            assert(parent_path != NULL);

            strncpy(parent_path, src_path, parent_path_len);
            parent_path[last_slash - src_path] = '\0';

            cd(shell, parent_path);  // Change directory to the parent directory
        }

        // Get the source file/dir
        char* file_name = last_slash == NULL ? src_path : last_slash + 1;

        File* src_file = dir_find_file(shell->current_directory, file_name);
        Directory* src_dir = dir_find_directory(shell->current_directory, file_name);
        if (src_file == NULL && src_dir == NULL) {
            printf("File %s does not exist\n", file_name);
            return;
        }

        if (src_file == NULL) {
            // Copy the directory
            Directory* copy = copy_dir(src_dir);

            // If mv, delete the original directory
            if (is_mv) {
                printf("success\n");
                rmdir(shell, src_dir->path);
            }

            // Go back to the original directory
            cd(shell, initial_path);

            // Go to the destination directory
            cd(shell, dest);

            // Put the copy in the destination directory
            copy->parent = shell->current_directory;
            add_elem(shell->current_directory->directories, copy);

            // Go back to the original directory
            cd(shell, initial_path);
        } else {
            // Copy the file
            File* copy = copy_file(src_file);

            // If mv, delete the original file
            if (is_mv) {
                rm(shell, src_file->path);
            }

            // Go back to the original directory
            cd(shell, initial_path);

            // Go to the destination directory
            cd(shell, dest);

            // Put the copy in the destination directory
            copy->parent = shell->current_directory;
            add_elem(shell->current_directory->files, copy);

            // Go back to the original directory
            cd(shell, initial_path);
        }
        free(initial_path);
    }
    
}
