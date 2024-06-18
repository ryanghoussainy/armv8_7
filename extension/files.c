#include "files.h"


char *get_file_path(const char* dir, const char* name)
{
    char* name_with_slash = malloc(sizeof(strlen(name) + 2));   // +2, 1 for the slash, 1 for the null terminator
    name_with_slash[0] = '/';
    strcpy(name_with_slash + 1, name);

    if (strcmp(dir, "/") == 0)
        return name_with_slash;
    
    char *result = malloc(strlen(dir) + strlen(name_with_slash) + 1);  // +1 for null terminator
    assert(result != NULL);
    strcpy(result, dir);
    strcat(result, name_with_slash);
    free(name_with_slash);
    return result;
}

/*
Returns the path of the parent directory of the given path.
Assumes that the path is not the root, i.e. that it has a parent.
*/
char* previous_directory_path(char* path) {
    assert(path != NULL);

    char* last_slash = strrchr(path, '/');

    if (last_slash == path) {
        return strdup("/");
    }

    char* result = malloc(last_slash - path + 1);  // last_slash - path gives the number of elems between them
    strncpy(result, path, last_slash - path);
    result[last_slash - path] = '\0';
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

    new_file->content = NULL;

    new_file->path = get_file_path(dir->path, name);

    new_file->parent = dir;

    add_elem(dir->files, new_file);

    return new_file;
}


Directory* create_dir(Directory* dir, char* name) {
    Directory* new_dir = malloc(sizeof(Directory));
    new_dir->name = strdup(name);

    new_dir->files = create_linked_list((FreeFunc)free_file);
    new_dir->directories = create_linked_list((FreeFunc)free_dir);

    new_dir->path = get_file_path(dir->path, name);

    new_dir->parent = dir;

    add_elem(dir->directories, new_dir);

    return new_dir;
}

void file_write(File* file, char* content, bool append) {
    if (append) {
        file->content = realloc(file->content, strlen(file->content) + strlen(content) + 1);
        strcat(file->content, content);
    } else {
        free(file->content);
        file->content = strdup(content);
    }
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

/*
Returns the file with the given name inside of a given directory, or NULL if not found.
*/
File* dir_find_file(Directory* dir, char* name) {
    assert(dir != NULL);
    assert(name != NULL);

    // Loop through files to get the file with the given name
    Node* current_node = dir->files->head;
    while (current_node != NULL) {
        File* current_file = (File*)current_node->elem;
        if (strcmp(current_file->name, name) == 0) {
            return current_file;
        }
        current_node = current_node->next;
    }
    return NULL;
}

/*
Returns the directory with the given name inside of a given directory, or NULL if not found.
*/
Directory* dir_find_directory(Directory* dir, char* name) {
    assert(dir != NULL);
    assert(name != NULL);

    // Loop through directories to get the directory with the given name
    Node* current_node = dir->directories->head;
    while (current_node != NULL) {
        Directory* current_dir = (Directory*)current_node->elem;
        if (strcmp(current_dir->name, name) == 0) {
            return current_dir;
        }
        current_node = current_node->next;
    }
    return NULL;
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
