#pragma once
#include <stdlib.h>
#include <assert.h>


struct File;

typedef void (*FreeFunc)(void*);

typedef struct {
    void* elem;
    LinkedList* next;
    FreeFunc free_elem;
} LinkedList;
