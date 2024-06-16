#include "files.h"

void free_file(File* file) {
    assert(file != NULL);

    free(file->name);
    free(file->content);
    free(file);
}
