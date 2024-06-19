#include "branch-asm.h"

static uint32_t conditional_instruction(Instruction* instr) {
    char condition[NUM_CHARS_COND] = {instr->operation[2], instr->operation[3]};

    uint32_t cond;

    if (strcmp(condition, "eq") == 0) {
        cond = COND_EQ;
    } else if (strcmp(condition, "ne") == 0) {
        cond = COND_NE;
    } else if (strcmp(condition,"ge") == 0) {
        cond = COND_GE;
    } else if (strcmp(condition,"lt") == 0) {
        cond = COND_LT;
    } else if (strcmp(condition, "gt") == 0) {
        cond = COND_GT;
    } else if (strcmp(condition, "le") == 0) {
        cond = COND_LE;
    } else if (strcmp(condition, "al") == 0) {
        cond = COND_AL;
    } else {
        printf("Invalid branch condition");
        return 0;
    }

    uint32_t simm19 = instr->o1.number;
    simm19 = (build_mask(SIMM19_BITS) & simm19) << SIMM19_SHIFT;
    return (uint32_t) (cond + simm19 + ((uint32_t) COND_TOP_BITS));
}

static uint32_t register_instruction(Instruction* instr) {
    bool is64Bit;
    return (uint32_t) ((register_number(instr->o1.reg, &is64Bit) << XN_SHIFT) + ((uint32_t) REG_TOP_BITS));
}

static uint32_t unconditional_instruction(Instruction* instr) {
    uint32_t address_to_jump_to = (uint32_t) (build_mask(SIMM26_BITS) & (instr->o1.number));
    return (uint32_t) (((uint32_t) UNCOND_TOP_BITS) + address_to_jump_to);
}

uint32_t branch_assembly(Instruction* instr) {
    if (instr->operation[1] == '.') {
        return conditional_instruction(instr);
    } else if (instr->operation[1] == 'r') {
        return register_instruction(instr);
    } else if (instr->operation[0] == 'b' && instr->operation[1] == '\0') {
        return unconditional_instruction(instr);
    } else {
        printf("Invalid Instruction passed into branch");
        return 0;
    }
}
