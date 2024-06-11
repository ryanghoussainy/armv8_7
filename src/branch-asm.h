#pragma once
#include "parse-asm.h"
#include "instr-utils.h"

#define COND_EQ 0
#define COND_NE 1
#define COND_GE 10
#define COND_LT 11
#define COND_GT 12
#define COND_LE 13
#define COND_AL 14

#define SIMM19_BITS 0, 18
#define SIMM19_SHIFT 5

#define COND_TOP_BITS 84 << 24
#define REG_TOP_BITS 54815 << 16
#define UNCOND_TOP_BITS 5 << 26

#define SIMM26_BITS 0, 25

#define XN_SHIFT 5

#define NUM_CHARS_COND 2

uint32_t branch_assembly(Instruction* instr);
