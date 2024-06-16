#include "files.h"

File* create_file(char* name, char* content, int size, char* path) {
    assert(name != NULL);
    assert(content != NULL);
    assert(path != NULL);

    File* result = malloc(sizeof(File));
    assert(result != NULL);

    result->name = malloc(strlen(name) + 1);
    assert(result->name != NULL);
    strcpy(result->name, name);

    result->content = malloc(strlen(content) + 1);
    assert(result->content != NULL);
    strcpy(result->content, content);

    result->size = size;

    result->path = malloc(strlen(path) + 1);
    assert(result->path != NULL);
    strcpy(result->path, path);

    return result;
}

Directory* create_directory(char* name, char* path) {
    assert(name != NULL);
    assert(path != NULL);

    Directory* result = malloc(sizeof(Directory));
    assert(result != NULL);

    result->name = malloc(strlen(name) + 1);
    assert(result->name != NULL);
    strcpy(result->name, name);

    result->files = create_linked_list(&free_file);
    result->directories = create_linked_list(&free_dir);

    result->path = malloc(strlen(path) + 1);
    assert(result->path != NULL);
    strcpy(result->path, path);

    return result;
}

void dir_add_file(Directory* dir, File* file) {
    assert(dir != NULL);
    assert(file != NULL);

    add_elem(dir->files, file);
}

void dir_add_directory(Directory* dir, Directory* new_dir) {
    assert(dir != NULL);
    assert(new_dir != NULL);

    add_elem(dir->directories, new_dir);
}

/*
Returns 1 for success, 0 for failure.
*/
int dir_remove_file(Directory* dir, File* file) {
    assert(dir != NULL);
    assert(file != NULL);

    return remove_elem(dir->files, file);
}

/*
Returns 1 for success, 0 for failure.
*/
int dir_remove_directory(Directory* dir, Directory* rm_dir) {
    assert(dir != NULL);
    assert(rm_dir != NULL);

    return remove_elem(dir->directories, rm_dir);
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
