#include "linked-list.h"


LinkedList* create_linked_list(FreeFunc free_elem) {
    LinkedList* result = malloc(sizeof(LinkedList));
    assert(result != NULL);

    result->elem = NULL;
    result->next = NULL;
    result->free_elem = free_elem;

    return result;
}

void add_elem(LinkedList* list, void* elem) {
    assert(list != NULL);

    LinkedList* current = list;

    // Go to end of list
    while (current->next != NULL) {
        current = current->next;
    }

    // Add new element
    current->next = malloc(sizeof(LinkedList));
    assert(current->next != NULL);

    current->next->elem = elem;
    current->next->next = NULL;
}

/*

Returns 1 for success, 0 for failure.
*/
int remove_elem(LinkedList* list, void* elem) {
    assert(list != NULL);

    LinkedList* current = list;
    LinkedList* previous = NULL;

    while (current != NULL) {
        if (current->elem == elem) {
            // Element found
            if (previous == NULL) {
                // Removing first element
                list = current->next;
            }
            else {
                previous->next = current->next;
            }
            free_elem(current->elem);
            free(current);

            return 1;
        }
        previous = current;
        current = current->next;
    }
}

void free_linked_list(LinkedList* list) {
    assert(list != NULL);
    assert(list->free_elem != NULL);

    free_elem(list->elem);
    if (list->next != NULL) free_linked_list(list->next);
    free(list);
}
