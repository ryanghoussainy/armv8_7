#include "dp-imm-assemble.h"

static uint32_t build_dp_imm_arithmetic(Instruction* instr) 
{
    uint32_t opi = ARITH_OPI;
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
    uint32_t sh = instr->o4_type == LSL && instr->o4.number == SH_SHIFT_AMOUNT;

    return (sf << SF_SHIFT) | (opc << OPC_SHIFT) | MIDDLE_BITS | (opi << OPI_SHIFT) | (sh << SH_SHIFT) | (imm12 << IMM12_SHIFT) | (rn << RN_SHIFT) | rd;
}

static uint32_t build_dp_imm_logical(Instruction* instr)
{
    uint32_t opi = LOGICAL_OPI;
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
        hw = shift / HW_FACTOR;
    }

    return (sf << SF_SHIFT) | (opc << OPC_SHIFT) | MIDDLE_BITS | (opi << OPI_SHIFT) | (hw << HW_SHIFT) | (imm16 << IMM16_SHIFT) | rd;
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