#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

enum Operation {
    NONE,
    LS,
    MAN,
    CD,
    TOUCH,
    MKDIR
    // Add more as the number of instructions increase
};

typedef struct {
    enum Operation operation;
    char* options;
    char** arguments;
    size_t argument_count;
    char* manual;
} Instruction;

// Format: operation -(options) argument1 argument2 ...

char** split_string(char str[], const char* sep, size_t* word_count);
enum Operation parse_to_operation(char* operation);
Instruction parse_to_instruction(char* instruction);
int free_instruction(Instruction* instruction);