#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Entry {
    // linked-list style map from string to int
    char* label;
    int value;
    struct Entry* next;
} Entry;


int add_entry(Entry* map, char* label, int value);
int get_value(Entry* map, char* label);
