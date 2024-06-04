#include "dp-imm-assemble.h"

uint32_t build_dp_imm(Instruction instruction)
{
    char* operation = instruction.operation;

    if (strstr(operation, "add") != NULL  || strstr(operation, "sub") != NULL) {
        return build_dp_imm_arithmetic(instruction);
    }else {
        return build_dp_imm_logical(instruction);
    }
}

uint32_t build_dp_imm_arithmetic(Instruction instruction) 
{
    uint32_t opi = 2;
    uint32_t opc;
    char* operation = instruction.operation;

    if (strcmp(operation, "add") == 0) {
        opc = 0;
    }else if (strcmp(operation, "adds") == 0) {
        opc = 1;
    }else if (strcmp(operation, "sub") == 0) {
        opc = 2;
    }else if (strcmp(operation, "subs") == 0) {
        opc = 3;
    }

    char* rd_str = instruction.o1.reg;
    uint32_t sf = rd_str[0] == 'x';
    uint32_t rd = atoi(rd_str + 1);

    char* rn_str = instruction.o2.reg;
    uint32_t rn = atoi(rn_str + 1);

    uint32_t imm12 = instruction.o3.number;
    uint32_t sh = 0;

    if (instruction.o4_type == LSL) {
        sh = instruction.o4.number;
    }

    return (sf << 31) | (opc << 29) | (4 << 26) | (opi << 23) | (sh << 22) | (imm12 << 10) | (rn << 5) | rd;
}

uint32_t build_dp_imm_logical(Instruction instruction)
{
    uint32_t opi = 3;
    uint32_t opc;
    char* operation = instruction.operation;

    if (strcmp(operation, "movn") == 0) {
        opc = 0;
    }else if (strcmp(operation, "movz") == 0) {
        opc = 2;
    }else if (strcmp(operation, "movk") == 0) {
        opc = 3;
    }

    char* rd_str = instruction.o1.reg;
    uint32_t sf = rd_str[0] == 'x';
    uint32_t rd = atoi(rd_str + 1);

    uint32_t imm16 = instruction.o2.number;
    uint32_t hw;

    if (instruction.o3_type == LSL) {
        int shift = instruction.o3.number;
        hw = shift / 16;
    }

    return (sf << 31) | (opc << 29) | (4 << 26) | (opi << 23) | (hw << 21) | (imm16 << 5) | rd;
}