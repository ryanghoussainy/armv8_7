#pragma once
#include <stdlib.h>
#include <assert.h>


typedef void (*FreeFunc)(void*);

struct LinkedList {
    void* elem;
    struct LinkedList* next;
    FreeFunc free_elem;
};

typedef struct LinkedList LinkedList;
