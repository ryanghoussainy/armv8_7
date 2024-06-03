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

enum LINE_TYPE {
    INSTRUCTION,
    LABEL,
    DIRECTIVE
};


enum INSTRUCTION_TYPE {
    DP_IMMEDIATE,
    DP_REGISTER,
    TRANSFER,
    BRANCH
};


enum OPERAND_TYPE {
    REGISTER,
    LITERAL,
    // only used to map the integer value back into the operand
    ADDRESS,
    // used to indicate operand is not in use
    NONE
};

union Operand {
    char reg[4];
    int literal;
};

typedef struct {
    // the memory this instruction is stored in
    uint64_t memory;

    // length could be set to 5
    char operation[8];
    
    union Operand o1;
    enum OPERAND_TYPE o1_type;

    union Operand o2;
    enum OPERAND_TYPE o2_type;

    union Operand o3;
    enum OPERAND_TYPE o3_type;

    union Operand o4;
    enum OPERAND_TYPE o4_type;

} Instruction;

void remove_leading_spaces(char* str);
char last_character(const char* str);
void remove_last_character(char* str);
char** split_string(char str[], const char* sep, size_t* word_count);
uint64_t register_number(const char* str, bool* is_64_bit);
enum LINE_TYPE classify_line(char str[]);
union Operand build_operand(char* str, Entry* map);
Instruction build_instruction(char* str, Entry* map);
