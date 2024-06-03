#include <stdint.h>
#include <stdio.h>
#include "instr-utils.h"
#include "cpu.h"

int conditional_branch(CPU* cpu, uint32_t instr);
int register_branch(CPU* cpu, uint32_t instr);
int unconditional_branch(CPU* cpu, uint32_t instr);
int branch_instruction(CPU* cpu, uint32_t instr);
