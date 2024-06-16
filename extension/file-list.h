#pragma once
#include "files.h"


struct File;

typedef struct {
    struct File* file;
    struct FileList* next;
} FileList;
