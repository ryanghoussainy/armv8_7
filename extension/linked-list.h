#pragma once
#include <stdlib.h>
#include <assert.h>


typedef void (*FreeFunc)(void*);

struct Node {
    void* elem;
    struct Node* next;
};

typedef struct Node Node;

struct LinkedList {
    Node* head;
    Node* tail;
    int size; 
    FreeFunc free_elem;
};
typedef struct LinkedList LinkedList;

LinkedList* create_linked_list(FreeFunc free_elem);
void add_elem(LinkedList* list, void* elem);
void free_linked_list(LinkedList* list);