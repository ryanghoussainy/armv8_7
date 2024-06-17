#include "files.h"

ELEMENT_TYPE identify_type(void* item) {
    if (sizeof(item) == sizeof(File)) {
        return FILE;
    } else if (sizeof(item) == sizeof(Directory)) {
        return DIRECTORY;
    } else {
        // error
        printf("neither a file or a directory");
        exit(1);
    }
}

void free_file(File* file) {
    assert(file != NULL);

    free(file->name);
    free(file->content);
    free(file->path);
    free(file);
}

void free_dir(Directory* dir) {
    assert(dir != NULL);

    free(dir->name);
    if (dir->files != NULL)       dir->files->free_elem(dir->files);
    if (dir->directories != NULL) dir->directories->free_elem(dir->directories);
    free(dir->path);
    free(dir);
}
