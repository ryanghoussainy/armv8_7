#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "linked-list.h"


typedef enum ELEMENT_TYPE {
    SFILE,
    SDIRECTORY,
} ELEMENT_TYPE;

typedef struct
{
    char *name;
    char name_size;
    char *content;
    int size;
    char *path;
    int path_size;
} File;

typedef struct
{
    char *name;
    char name_size;
    LinkedList* files;
    LinkedList* directories;
    char *path;
    int path_size;
} Directory;

char* get_file_path(const char* dir, const char* name);
ELEMENT_TYPE identify_type(void* item);
File* create_file(Directory* dir, char* name);
Directory* create_dir(Directory* dir, char* name);
void dir_add_file(Directory* dir, File* file);
int remove_elem(LinkedList* list, void* elem);
void free_file(File* file);
void free_dir(Directory* dir);
