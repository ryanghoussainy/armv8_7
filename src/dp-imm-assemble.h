#pragma once
#include <stdint.h>
#include "parse-asm.h"

#define SF_SHIFT 31
#define OPC_SHIFT 29
#define OPI_SHIFT 23
#define SH_SHIFT 22
#define IMM12_SHIFT 10
#define RN_SHIFT 5
#define HW_SHIFT 21
#define IMM16_SHIFT 5

#define MIDDLE_BITS 4 << 26

#define SH_SHIFT_AMOUNT 12

#define ARITH_OPI 2
#define LOGICAL_OPI 5

#define HW_FACTOR 16

uint32_t build_dp_imm(Instruction* instruction);
