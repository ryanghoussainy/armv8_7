#include "dp-imm-assemble.h"

static uint32_t build_dp_imm_arithmetic(Instruction* instr) 
{
    uint32_t opi = 2;
    uint32_t opc;
    char* operation = instr->operation;

    if (strcmp(operation, "add") == 0) {
        opc = 0;
    }else if (strcmp(operation, "adds") == 0) {
        opc = 1;
    }else if (strcmp(operation, "sub") == 0) {
        opc = 2;
    }else if (strcmp(operation, "subs") == 0) {
        opc = 3;
    }

    char* rd_str = instr->o1.reg;
    bool sf;
    uint32_t rd = register_number(rd_str, &sf);

    char* rn_str = instr->o2.reg;
    uint32_t rn = register_number(rn_str, &sf);

    uint32_t imm12 = instr->o3.number;
    uint32_t sh = instr->o4_type == LSL && instr->o4.number == 12;

    return (sf << 31) | (opc << 29) | (4 << 26) | (opi << 23) | (sh << 22) | (imm12 << 10) | (rn << 5) | rd;
}

static uint32_t build_dp_imm_logical(Instruction* instr)
{
    uint32_t opi = 5;
    uint32_t opc;
    char* operation = instr->operation;

    if (strcmp(operation, "movn") == 0) {
        opc = 0;
    }else if (strcmp(operation, "movz") == 0) {
        opc = 2;
    }else if (strcmp(operation, "movk") == 0) {
        opc = 3;
    }

    char* rd_str = instr->o1.reg;
    bool sf;
    uint32_t rd = register_number(rd_str, &sf);

    uint32_t imm16 = instr->o2.number;
    uint32_t hw = 0;

    if (instr->o3_type == LSL) {
        int shift = instr->o3.number;
        hw = shift / 16;
    }

    return (sf << 31) | (opc << 29) | (4 << 26) | (opi << 23) | (hw << 21) | (imm16 << 5) | rd;
}

uint32_t build_dp_imm(Instruction* instr)
{
    char* operation = instr->operation;

    if (strstr(operation, "add") != NULL  || strstr(operation, "sub") != NULL) {
        return build_dp_imm_arithmetic(instr);
    }else {
        return build_dp_imm_logical(instr);
    }
}