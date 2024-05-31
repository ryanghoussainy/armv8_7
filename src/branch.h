#include <stdint.h>
#include <stdio.h>
#include "masks.h"
#include "cpu.h"

int conditional_branch(CPU* cpu, uint32_t instr);
int register_branch(CPU* cpu, uint32_t instr);
int unconditional_branch(CPU* cpu, uint32_t instr);
int branch_instruction(CPU* cpu, uint32_t instr);

uint64_t sign_extend(uint32_t num, int num_bits);
