#pragma once
#include "arithmetic-op.h"

#define SF_BITS      31, 31
#define OPC_BITS     29, 30
#define M_BITS       28, 28
#define OPR_BITS     21, 24
#define RM_BITS      16, 20
#define OPERAND_BITS 10, 15
#define RN_BITS      5, 9
#define RD_BITS      0, 4
#define SHIFT_BITS   22, 23
#define N_BITS       21, 21
#define X_BITS       15, 15
#define RA_BITS      10, 14

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
