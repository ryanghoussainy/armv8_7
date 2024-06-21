#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

enum Operation {
    NONE,
    CAT,
    CD,
    CP,
    ECHO,
    LS,
    MAN,
    MKDIR,
    MV,
    PWD,
    RM,
    RMDIR,
    TOUCH
};

typedef struct {
    enum Operation operation;
    char** arguments;
    size_t argument_count;
    char* manual;
} Command;

// Format: operation -(options) argument1 argument2 ...

char** split_string(char str[], size_t* word_count);
void output_command(Command* command);
enum Operation parse_to_operation(char* operation);
Command* parse_to_command(char* command);
char* get_manual(enum Operation operation);
void free_command(Command* command);