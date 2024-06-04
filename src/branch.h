#include "cpu.h"

#define OFFSET_MULT 4

#define SIMM19_NUM_BITS 19
#define SIMM26_NUM_BITS 26

#define XN_SHIFT            5
#define BRANCH_SIMM19_SHIFT 5
#define INSTR_UNCOND_SHIFT  26
#define INSTR_10TO31_SHIFT  10
#define INSTR_24TO31_SHIFT  24
#define INSTR_4_SHIFT       4

#define UNCOND_BITS     0, 25
#define REG_BITS        5, 9
#define COND_INDEX_BITS 5, 23
#define COND_BITS       0, 3

#define INSTR_UNCOND_BITS 26, 31
#define INSTR_REG_1       0, 4
#define INSTR_REG_2       10, 31
#define INSTR_COND_BITS   24, 31

#define REG_BRANCH_10TO31    3508160
#define COND_BRANCH_24TO31   84
#define UNCOND_BRANCH_26TO31 5

int branch_instruction(CPU* cpu, uint32_t instr);
