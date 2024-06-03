#pragma once
#include "arithmetic-op.h"

typedef struct {
    uint64_t sf;
    uint64_t opc;
    uint64_t M;
    uint64_t opr;
    uint64_t rm;
    uint64_t operand;
    uint64_t rn;
    uint64_t rd;
    uint64_t shift;
    uint64_t N;
    uint64_t x;
    uint64_t ra;
} DPRegComponents;

int dp_reg_instruction(CPU* cpu, uint32_t instr);
