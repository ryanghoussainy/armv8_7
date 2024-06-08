#include "branch-asm.h"

uint32_t conditional_instruction(Instruction* instr) {
    char condition[2] = {instr->operation[2], instr->operation[3]};

    uint32_t cond = 15;
    // switch doesn't work on strings in C so confined to ifs

    if (strcmp(condition, "eq") == 0) {
        cond = 0;
    } else if (strcmp(condition, "ne") == 0) {
        cond = 1;
    } else if (strcmp(condition,"ge") == 0) {
        cond = 10;
    } else if (strcmp(condition,"lt") == 0) {
        cond = 11;
    } else if (strcmp(condition, "gt") == 0) {
        cond = 12;
    } else if (strcmp(condition, "le") == 0) {
        cond = 13;
    } else if (strcmp(condition, "al") == 0) {
        cond = 14;
    } else {
        printf("Invalid branch condition");
        return 0;
    }

    uint32_t simm19 = instr->o1.number;
    simm19 = (build_mask(0, 18) & simm19) << 5; // might need to sign de-extend?
    return (uint32_t) (cond + simm19 + ((uint32_t)84 << 24));
}

uint32_t register_instruction(Instruction* instr) {
    bool is64Bit;
    return (uint32_t) ((register_number(instr->o1.reg, &is64Bit) << 5) + ((uint32_t)54815 << 16));
}

uint32_t unconditional_instruction(Instruction* instr) {
    uint32_t address_to_jump_to = (uint32_t) (build_mask(0, 25) & (instr->o1.number));
    return (uint32_t) (((uint32_t)5 << 26) + address_to_jump_to);
}

uint32_t branch_assembly(Instruction* instr) {
    //print_instruction(instr); // for debugging
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
