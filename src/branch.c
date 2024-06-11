#include "branch.h"


static int unconditional_branch(CPU* cpu, uint32_t instr) {
    
    uint32_t mask = build_mask(UNCOND_BITS);

    uint32_t masked = mask & instr;

    uint64_t offset = sign_extend(masked, SIMM26_NUM_BITS) * OFFSET_MULT;

    cpu->PC += offset;

    return 1;
}

static int register_branch(CPU* cpu, uint32_t instr) {

    uint32_t mask = build_mask(REG_BITS);

    uint32_t reg = (mask & instr) >> XN_SHIFT;

    cpu->PC = read_register(cpu, reg, 1);
    return 1;
}

static int conditional_branch(CPU* cpu, uint32_t instr) {

    uint32_t mask_index = build_mask(COND_INDEX_BITS);
    uint32_t mask_cond = build_mask(COND_BITS);

    uint32_t cond = mask_cond & instr;
    int cond_holds = 0;

    switch (cond) {
        case COND_EQ: // Z == 1
            cond_holds = (cpu->PSTATE.Z == 1);
            break;
        case COND_NE: // Z == 0
            cond_holds = (cpu->PSTATE.Z == 0);
            break;
        case COND_GE: // N == V
            cond_holds = (cpu->PSTATE.N == cpu->PSTATE.V);
            break;
        case COND_LT: // N != V
            cond_holds = (cpu->PSTATE.N != cpu->PSTATE.V);
            break;
        case COND_GT: // Z == 0 && N == V
            cond_holds = (cpu->PSTATE.N == cpu->PSTATE.V) && (cpu->PSTATE.Z == 0);
            break;
        case COND_LE: // !(Z == 0 && N == V)
            cond_holds = !((cpu->PSTATE.N == cpu->PSTATE.V) && (cpu->PSTATE.Z == 0));
            break;
        case COND_AL: // always
            cond_holds = 1;
            break;
        default:
            printf("Invalid condition");
            return 0;
    }

    if (cond_holds) {

        uint32_t masked = (mask_index & instr) >> BRANCH_SIMM19_SHIFT;

        uint64_t offset = sign_extend(masked, SIMM19_NUM_BITS) * OFFSET_MULT;

        cpu->PC += offset;
    }else {
        cpu->PC += 4;
    }

    return 1;
}

int branch_instruction(CPU* cpu, uint32_t instr) {

    // Large numbers in conditions (5, 508160, 84) correspond to opcode in spec
    // Identify correct branch through isolating relevant bits w/ masks

    if (cpu == NULL) {
        printf("Cpu parameter is null.\n");
        return 1;
    }

    uint32_t uncon_mask = build_mask(INSTR_UNCOND_BITS);
    uint32_t reg_mask_one = build_mask(INSTR_REG_1);
    uint32_t reg_mask_two = build_mask(INSTR_REG_2);
    uint32_t cond_mask = build_mask(INSTR_COND_BITS);

    int is_uncon = (((uncon_mask & instr) >> INSTR_UNCOND_SHIFT) == UNCOND_BRANCH_26TO31);

    if (is_uncon) {

        return unconditional_branch(cpu, instr);

    } else {

        int is_reg = ((reg_mask_one & instr) == 0) && (((reg_mask_two & instr) >> INSTR_10TO31_SHIFT) == REG_BRANCH_10TO31);

        if (is_reg) {

            return register_branch(cpu, instr);

        } else {

            int is_cond = (((cond_mask & instr) >> INSTR_24TO31_SHIFT) == COND_BRANCH_24TO31) && (((instr >> INSTR_4_SHIFT) & 1) == 0);

            if (is_cond) {
                return conditional_branch(cpu, instr);
            } else {

                printf("Invalid branch instruction format");

                return 0;
            }
        }
    }

    return 1;
}
