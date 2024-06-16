#pragma once
#include <stdlib.h>
#include <assert.h>


struct File;

typedef struct {
    struct File* file;
    FileList* next;
} FileList;
