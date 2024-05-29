#pragma once
#include "cpu.h"
#include <stdint.h>

int execute_instruction(struct CPU* cpu, uint32_t instruction);

void cycle(struct CPU* cpu);