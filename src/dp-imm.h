#pragma once
#include "cpu.h"

struct DPImmComponents {
    uint64_t sf;
    uint64_t opc;
    uint64_t opi;
    uint64_t rd;
    uint64_t sh;
    uint64_t imm12;
    uint64_t rn;
    uint64_t hw;
    uint64_t imm16;
};

struct DPImmComponents get_components(uint32_t instruction);
void dp_imm_instruction(struct CPU* cpu, uint32_t instruction);
void do_arithmetic(struct CPU* cpu, struct DPImmComponents* components);
void do_wide_move(struct CPU* cpu, struct DPImmComponents* components);