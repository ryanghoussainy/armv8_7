#pragma once
#include "cpu.h"
#include "arithmetic-op.h"

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

DPImmComponents get_dp_imm_components(uint32_t instruction);
int dp_imm_instruction(CPU* cpu, uint32_t instruction);
int do_arithmetic(CPU* cpu, DPImmComponents* components);
int do_wide_move(CPU* cpu, DPImmComponents* components);
