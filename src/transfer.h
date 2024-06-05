#pragma once
#include <stdbool.h>
#include "branch.h"

#define TRANSFER_SIMM19_SHIFT 2
#define SIMM19_NUM_BITS       19

#define NUM_BYTES 4

#define SIMM9_NUM_BITS 9

#define UOFFSET_SHIFT 2

#define OFFSET_SIMM9_BITS 2, 10
#define OFFSET_I_BITS     1, 1

#define INSTR_RT_BITS         0, 4
#define INSTR_SF_BITS         30, 30
#define INSTR_U_BITS          24, 24
#define INSTR_L_BITS          22, 22
#define INSTR_OFFSET_BITS     10, 21
#define INSTR_XN_BITS         5, 9
#define INSTR_TOP_OFFSET_BITS 21, 21
#define INSTR_XM_BITS         16, 20
#define INSTR_SIMM19_BITS     5, 23
#define INSTR_TOP_BITS        31, 31


int transfer_instruction(CPU* cpu, uint32_t instr);
