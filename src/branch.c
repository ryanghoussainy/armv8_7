#include "branch.h"

int branch_instruction(CPU* cpu, uint32_t instr) {

    // Large numbers in conditions (5, 508160, 84) correspond to opcode in spec

    uint32_t uncon_mask = build_mask(26, 31);
    uint32_t reg_mask_one = build_mask(0, 4);
    uint32_t reg_mask_two = build_mask(10, 31);
    uint32_t cond_mask = build_mask(24, 31);

    int isUncon = (((uncon_mask & instr) >> 26) == 5);

    if (isUncon) {
        return unconditional_branch(cpu, instr);
    } else {

        int isReg = ((reg_mask_one & instr) == 0) && (((reg_mask_two & instr) >> 10) == 3508160);

        if (isReg) {

            return register_branch(cpu, instr);

        } else {

            int isCond = (((cond_mask & instr) >> 24) == 84) && (((instr >> 4) & 1) == 0);

            if (isCond) {
                return conditional_branch(cpu, instr);
            } else {

                printf("Invalid branch instruction format");

                return 0;
            }
        }
    }

    return 1;
}

int unconditional_branch(CPU* cpu, uint32_t instr) {
    
    uint32_t mask = build_mask(0, 25);

    uint32_t masked = mask & instr;

    uint64_t offset = sign_extend(masked, 26) * 4;

    cpu->PC += offset;

    return 1;
}

int register_branch(CPU* cpu, uint32_t instr) {

    uint32_t mask = build_mask(5, 9);

    uint32_t reg = (mask & instr) >> 5;

    cpu->PC = read_register(cpu, reg, 1);
    return 1;
}

int conditional_branch(CPU* cpu, uint32_t instr) {

    uint32_t mask_index = build_mask(5, 23);
    uint32_t mask_cond = build_mask(0, 3);

    uint32_t cond = mask_cond & instr;
    int cond_holds = 0;

    switch (cond) {
        case 0: // Z == 1
            cond_holds = (cpu->PSTATE.Z == 1);
            break;
        case 1: // Z == 0
            cond_holds = (cpu->PSTATE.Z == 0);
            break;
        case 10: // N == V
            cond_holds = (cpu->PSTATE.N == cpu->PSTATE.V);
            break;
        case 11: // N != V
            cond_holds = (cpu->PSTATE.N != cpu->PSTATE.V);
            break;
        case 12: // Z == 0 && N == V
            cond_holds = (cpu->PSTATE.N == cpu->PSTATE.V) && (cpu->PSTATE.Z == 0);
            break;
        case 13: // !(Z == 0 && N == V)
            cond_holds = !((cpu->PSTATE.N == cpu->PSTATE.V) && (cpu->PSTATE.Z == 0));
            break;
        case 14: // always
            break;
        default:
            printf("Invalid condition");
            return 0;
    }

    if (cond_holds) {

        uint32_t masked = mask_index & instr;

        uint64_t offset = sign_extend(masked, 19) * 4;

        cpu->PC += offset;
    }

    return 1;
}

uint64_t sign_extend(uint32_t num, int num_bits) {
    int MSB = (num >> (num_bits - 1)) & 1;

    if (MSB) {
        uint64_t neg_mask = build_mask(num_bits, 63);
        return neg_mask | num;
    } else {
        return (uint64_t) num;
    }

    return 0;
}
