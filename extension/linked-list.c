#include "linked-list.h"
#include "files.h"
#include <stdio.h>
#include <string.h>

LinkedList* create_linked_list(FreeFunc free_elem) {
    LinkedList* linked_list = (LinkedList*) malloc(sizeof(LinkedList));
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
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->elem = elem;
    new_node->next = NULL;

    assert(list != NULL);
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        list->tail->next = new_node;
    }
    list->tail = new_node;
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
    } else if (list->head->elem == elem) {
        // first element matches
        Node* temp = list->head->next;
        list->free_elem(list->head->elem);
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
            if (list->tail == current) {
                // last element matches
                list->tail = previous; 
            }
            previous->next = current->next;
            list->free_elem(current->elem);
            free(current);

            list->size--;
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

LinkedList* copy_linked_list(LinkedList* list) {
    assert(list != NULL);

    LinkedList* new_list = create_linked_list(list->free_elem);

    Node* current = list->head;

    if (current == NULL) {
        return new_list;
    }


    if (identify_type(current->elem) == FILET) {
        Directory* parent = ((File*)current->elem)->parent;
        while (current != NULL) {
            File* file = (File*)current->elem;
            File* file_copy = copy_file(file);
            file_copy->parent = parent;
            add_elem(new_list, file_copy);
            current = current->next;
        }
    } else {
        Directory* parent = ((Directory*)current->elem)->parent;
        while (current != NULL) {
            Directory* dir = (Directory*)current->elem;
            Directory* dir_copy = copy_dir(dir);
            dir_copy->parent = parent;
            add_elem(new_list, dir_copy);
            current = current->next;
        }
    }
    return new_list;
}

void free_linked_list(LinkedList* list) {
    assert(list != NULL);
    assert(list->free_elem != NULL);

    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current->next;
        list->free_elem(current->elem);
        free(current);
        current = temp;
    }
    free(list);
}
