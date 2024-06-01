#pragma once
#include "cpu.h"
#include <stdint.h>

int execute_instruction(CPU* cpu, uint32_t instruction);

void cycle(CPU* cpu);
