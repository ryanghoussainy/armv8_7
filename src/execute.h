#pragma once
#include "cpu.h"
#include <stdint.h>
#include "instr-utils.h"

int execute_instruction(CPU* cpu, uint32_t instruction);

void cycle(CPU* cpu);
