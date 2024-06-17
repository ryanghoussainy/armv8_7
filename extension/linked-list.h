#pragma once
#include <stdlib.h>
#include <assert.h>


typedef void (*FreeFunc)(void*);


struct Node {
    void* elem;
    Node* next;
};

typedef struct Node Node;


struct LinkedList {
    Node* head;
    Node* tail;
    int size; 
    FreeFunc free_elem;
};
typedef struct LinkedList LinkedList;
