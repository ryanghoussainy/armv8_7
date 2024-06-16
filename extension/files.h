#pragma once
#include "file-list.h"


struct FileList;

typedef struct {
    char* name;
    struct FileList* files;
} File;

typedef struct {
    char* name;
    File* files;
    struct Directory* directories;
} Directory;
