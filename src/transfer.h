#pragma once
#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "instr-utils.h"

int transfer_instruction(CPU* cpu, uint32_t instr);
