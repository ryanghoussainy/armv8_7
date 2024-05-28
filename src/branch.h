#include <stdint.h>
#include <stdio.h>
#include "masks.h"
#include "cpu.h"

int branch_instruction(struct CPU* cpu, uint32_t instr);

uint64_t sign_extend(uint32_t num, int num_bits);