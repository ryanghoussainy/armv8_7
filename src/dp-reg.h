#pragma once
#include "cpu.h"

struct DPRegComponents {
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
};

struct DPRegComponents get_components(uint32_t instr);
void dp_reg_instruction(struct CPU* cpu, uint32_t instr);
void reg_arithmetic(struct CPU* cpu, struct DPRegComponents* components);
void reg_logical(struct CPU* cpu, struct DPRegComponents* components);
void reg_multiply(struct CPU* cpu, struct DPRegComponents* components);
uint32_t perform_shift(uint32_t shift, uint32_t rm, uint32_t operand);
uint32_t lsl(uint32_t rm, uint32_t operand);
uint32_t lsr(uint32_t rm, uint32_t operand);
uint32_t asr(uint32_t rm, uint32_t operand);
uint32_t ror(uint32_t rm, uint32_t operand);
