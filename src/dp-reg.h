#pragma once
#include "cpu.h"

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

DPRegComponents get_dp_reg_components(uint32_t instr);
int dp_reg_instruction(CPU* cpu, uint32_t instr);
int reg_arithmetic(CPU* cpu, DPRegComponents* components);
uint64_t arithmetic_operation(CPU* cpu, uint64_t sf, uint64_t opc, uint64_t Rn, uint64_t op2);
int reg_logical(CPU* cpu, DPRegComponents* components);
uint64_t logical_operation(CPU* cpu, uint64_t sf, uint64_t opc, uint64_t N, uint64_t Rn, uint64_t op2);
int reg_multiply(CPU* cpu, DPRegComponents* components);
uint64_t multiply_operation(CPU* cpu, uint64_t x, uint64_t Ra, uint64_t Rn, uint64_t Rm);
uint64_t perform_shift(uint64_t shift, uint64_t rm, uint64_t operand);
uint64_t lsl(uint64_t rm, uint64_t operand);
uint64_t lsr(uint64_t rm, uint64_t operand);
uint64_t asr(uint64_t rm, uint64_t operand);
uint64_t ror(uint64_t rm, uint64_t operand);
