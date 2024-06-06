#pragma once
#include "parse-asm.h"
#include "dp-reg-assemble.h"
#include "dp-imm-assemble.h"

uint32_t dp_assembly(Instruction* instr);
