#include "file-list.h"


FileList* create_file_list(void) {
    FileList* result = malloc(sizeof(FileList));
    assert(result != NULL);

    result->file = NULL;
    result->next = NULL;

    return result;
}

void add_file(FileList* list, struct File* file) {
    assert(list != NULL);

    FileList* current = list;

    // Go to end of list
    while (current->next != NULL) {
        current = current->next;
    }

    // Add new file
    current->next = malloc(sizeof(FileList));
    assert(current->next != NULL);

    current->next->file = file;
    current->next->next = NULL;
}

void free_file_list(FileList* list) {
    assert(list != NULL);

    free_file(list->file);
    if (list->next != NULL) free_file_list(list->next);
    free(list);
}
