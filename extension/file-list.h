#pragma once
#include "files.h"


typedef struct {
    File* file;
    FileList* next;
} FileList;
