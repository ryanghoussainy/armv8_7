#pragma once
#include <stdlib.h>
#include <assert.h>


struct FileList;

typedef struct {
    char* name;
    char* content;
    int size;
} File;

typedef struct {
    char* name;
    File* files;
    struct Directory* directories;
} Directory;
