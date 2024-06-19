#include "cat.h"

/*
'cat' prints the contents of a file

Future development will see more options and more capabilities
*/
void cat(Shell* shell, char* path) {

    // vv Using same code as in rm.c --> make a function?

    char* prefixed_filepath = NULL;

    if (path[0] != '/') {
        prefixed_filepath = malloc(strlen("./") + strlen(path) + 1);
        assert(prefixed_filepath != NULL);
        strcpy(prefixed_filepath, "./");
        prefixed_filepath = strcat(prefixed_filepath, path);

    } else {
        prefixed_filepath = strdup(path);
    }

    char* name = prefixed_filepath + strlen(path) + 1;

    File* cat_file = dir_find_file(shell->current_directory, name);

    // ^^ Using same code as in rm.c --> make a function?

    if (cat_file != NULL) {
        fwrite(cat_file->content, 1, strlen(cat_file->content) + 1, shell->out);
    } else {
        char message[25] = "Empty or Invalid File";
        fwrite(message, 1, 25, shell->out);
    }

    free(prefixed_filepath);
}