#pragma once
#include <stdint.h>
#include "parse-asm.h"

#define OPC_ADD 0
#define OPC_ADDS 1
#define OPC_SUB 2
#define OPC_SUBS 3

#define OPC_AND 0
#define N_AND 0
#define OPC_BIC 0
#define N_BIC 1
#define OPC_ORR 1
#define N_ORR 0
#define OPC_ORN 1
#define N_ORN 1
#define OPC_EOR 2
#define N_EOR 0
#define OPC_EON 2
#define N_EON 1
#define OPC_ANDS 3
#define N_ANDS 0
#define OPC_BICS 3
#define N_BICS 1

#define SHIFT_LSL 0
#define SHIFT_LSR 1
#define SHIFT_ASR 2
#define SHIFT_ROR 3
#define NO_SHIFT 0

#define ARITH_OPR 8 + (shift << 1)
#define LOGICAL_OPR (shift << 1) + N

#define SF_SHIFT 31
#define OPC_SHIFT 29
#define OPR_SHIFT 21
#define RM_SHIFT 16
#define OPERAND_SHIFT 10
#define RN_SHIFT 5
#define X_SHIFT 15
#define RA_SHIFT 10

#define ARITH_MIDDLE_BITS 5 << 25
#define LOGICAL_MIDDLE_BITS 5 << 25
#define MULTIPLY_MIDDLE_BITS 216 << 21

uint32_t build_dp_reg(Instruction* instr);
