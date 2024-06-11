#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "parse-asm.h"
#include "instr-utils.h"

#define TRANSFER_SF_SHIFT 30
#define LL_CONSTANT 24
#define LL_CONSTANT_SHIFT 24
#define SIMM19_SHIFT 5

#define SDT_INDICATOR_SHIFT 31
#define SDT_CONSTANT 28
#define SDT_CONSTANT_SHIFT 25
#define XN_SHIFT 5
#define L_SHIFT 22
#define U_SHIFT 24
#define OFFSET_SHIFT 10

#define XM_SHIFT 16
#define REG_OFS_CONSTANT 13
#define REG_OFS_CONSTANT_SHIFT 11

#define SIMM9_SHIFT 2
#define I_SHIFT 1


uint32_t transfer_assembly(Instruction* ins);
