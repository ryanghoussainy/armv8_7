#include "linked-list.h"


LinkedList* create_linked_list(FreeFunc free_elem) {
    LinkedList* linked_list = malloc(sizeof(LinkedList));
    assert(linked_list != NULL);

    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;
    linked_list->free_elem = free_elem;
    
    return linked_list;
}

/*
File file;
LinkedList* list;
add_elem(list, file);

Adds an element to the end of the linked list.
*/
void add_elem(LinkedList* list, void* elem) {
    Node new_node = { elem, NULL };
    assert(list != NULL);
    if (list->head == NULL) {
        list->head = &new_node;
    } else {
        list->tail->next = &new_node;
    }
    list->tail = &new_node;
    list->size++;
}

/*
File file;
LinkedList* list;
if (!remove_elem(list, file)) {
    // Element not in list - throw error?
}

Returns 1 for success, 0 for failure.
*/
int remove_elem(LinkedList* list, void* elem) {
    assert(list != NULL);

    if (list->head == NULL) {
        // empty list
        return 0;
    } else if (list->head == elem) {
        // first element matches
        Node* temp = list->head->next;
        list->free_elem(elem);
        free(list->head);
        list->head = temp;
        list->size--;
        return 1;
    }

    Node* current = list->head;
    Node* previous = NULL;

    while (current != NULL) {
        if (current->elem == elem) {
            assert(previous != NULL);  // as case handled above
            // element found
            if (list->tail == elem) {
                // last element matches
                list->tail = previous; 
                list->free_elem(current);
            }
            previous->next = current->next;
            list->free_elem(elem);
            free(current);

            list->size--;
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

void free_linked_list(LinkedList* list) {
    assert(list != NULL);
    assert(list->free_elem != NULL);

    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current->next;
        list->free_elem(current->elem);
        free(current);
        current = current->next;
    }
    free(list);
}
