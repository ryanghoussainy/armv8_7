#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "symbol-table.h"

#define ZR 31
#define PC 32

#define HEX_SIZE 16

enum LINE_TYPE {
    INSTRUCTION,
    LABEL,
    DIRECTIVE
};


enum INSTRUCTION_TYPE {
    DP,
    TRANSFER,
    BRANCH
};


enum OPERAND_TYPE {
    REGISTER,
    LITERAL,
    // 4 different types of shifts
    LSL,
    LSR,
    ASR,
    ROR,
    // only used to map the integer value back into the operand
    ADDRESS,
    // used to indicate operand is not in use
    NONE
};

union Operand {
    char reg[4];
    // represents either a literal or a shift
    int number;
};


enum TRANSFER_TYPE {
    UNSIGNED_OFFSET,
    REGISTER_OFFSET,
    PRE_INDEX,
    POST_INDEX,
    LOAD_LITERAL,
};


typedef struct {
    char operation[16];
    
    union Operand o1;
    enum OPERAND_TYPE o1_type;

    union Operand o2;
    enum OPERAND_TYPE o2_type;

    union Operand o3;
    enum OPERAND_TYPE o3_type;

    union Operand o4;
    enum OPERAND_TYPE o4_type;

} Instruction;

uint64_t string_to_int(char* str);
void remove_leading_spaces(char* str);
void remove_last_character(char* str);
char** split_string(char str[], const char* sep, size_t* word_count);
uint64_t register_number(const char* str, bool* is_64_bit);
enum LINE_TYPE classify_line(char str[]);
enum INSTRUCTION_TYPE classify_instruction(char* operation);
void free_2d_array(char** arr, size_t size);
Instruction build_instruction(char* str, Entry* map, uint64_t address);
void print_instruction(Instruction* instr);
