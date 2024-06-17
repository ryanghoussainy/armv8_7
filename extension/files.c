#include "files.h"


char *get_file_path(const char* dir, const char* name)
{
    char* name_with_slash = malloc(sizeof(strlen(name) + 2));   // +2, 1 for the slash, 1 for the null terminator
    name_with_slash[0] = '/';
    strcpy(name_with_slash + 1, name);

    char *result = malloc(strlen(dir) + strlen(name_with_slash) + 1);  // +1 for null terminator
    assert(result != NULL);
    strcpy(result, dir);
    strcat(result, name_with_slash);
    free(name_with_slash);
    return result;
}


ELEMENT_TYPE identify_type(void* item) {
    if (sizeof(item) == sizeof(File)) {
        return SFILE;
    } else if (sizeof(item) == sizeof(Directory)) {
        return SDIRECTORY;
    } else {
        // error
        printf("%s", "neither a file or a directory");
        exit(1);
    }
}


File* create_file(Directory* dir, char* name) {
    File* new_file = malloc(sizeof(File));
    new_file->name = strdup(name);    
    // TODO: namesize?? Why is it a char

    new_file->path = get_file_path(dir->path, name);
    // TODO: path size?

    add_elem(dir->files, new_file);

    free(name);
    return new_file;
}


Directory* create_dir(Directory* dir, char* name) {
    Directory* new_dir = malloc(sizeof(Directory));
    new_dir->name = strdup(name);
    // TODO: namesize?? Why is it a char

    new_dir->files = create_linked_list((FreeFunc)&free_file);
    new_dir->directories = create_linked_list((FreeFunc)&free_dir);

    new_dir->path = get_file_path(dir->path, name);
    // TODO: path size?

    add_elem(dir->directories, new_dir);

    free(name);
    return new_dir;
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
