#pragma once
#include "cpu.h"
#include "dp-imm.h"
#include "dp-reg.h"
#include "transfer.h"

#define OP0_BITS 4
#define HALT_INSTR 2315255808


void cycle(CPU* cpu);
