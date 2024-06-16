#include "files.h"

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
