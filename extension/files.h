#pragma once
#include <stdlib.h>
#include <assert.h>

struct LinkedList;

typedef struct
{
    char *name;
    char *content;
    int size;
    char *path;
    int path_size;
} File;

typedef struct
{
    char *name;
    File *files;
    struct Directory *directories;
    char *path;
    int path_size;
} Directory;
