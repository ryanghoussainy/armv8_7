#pragma once
#include "arithmetic-op.h"

#define SF_BITS    31, 31
#define OPC_BITS   29, 30
#define OPI_BITS   23, 25
#define RD_BITS    0, 4
#define SH_BITS    22, 22
#define IMM12_BITS 10, 21
#define RN_BITS    5, 9
#define HW_BITS    21, 22
#define IMM16_BITS 5, 20

#define IMM12_SHIFT 12
#define IMM16_VAL 16

typedef struct {
    uint64_t sf;
    uint64_t opc;
    uint64_t opi;
    uint64_t rd;
    uint64_t sh;
    uint64_t imm12;
    uint64_t rn;
    uint64_t hw;
    uint64_t imm16;
} DPImmComponents;

int dp_imm_instruction(CPU* cpu, uint32_t instruction);
