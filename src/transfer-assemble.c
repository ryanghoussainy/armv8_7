#include "transfer-assemble.h"


uint32_t transfer_assembly(Instruction* ins) {

    /* Operand 4 will be used to indicate the type of transfer and store */

    // L set to 0 if str, 1 if ldr
    bool L = strcmp(ins->operation, "str");
    bool sf;
    
    // operand 1 is always register

    if (ins->o1_type != REGISTER) {
        // error
        return EXIT_FAILURE;
    }

    if (ins->o4_type != LITERAL) {
        // error
        return EXIT_FAILURE;
    }

    uint64_t rt = register_number(ins->o1.reg, &sf);

    uint32_t result;

    switch(ins->o4.number) {
        uint32_t xn;
        uint32_t xm;

        // unused
        bool xn_64_bit;
        bool xm_64_bit; 

        uint16_t simm9;
        uint16_t imm12;

        case UNSIGNED_OFFSET:
            xn = register_number(ins->o2.reg, &xn_64_bit);
            if (ins->o3_type == LITERAL) {
                if (sf) {
                    imm12 = ins->o3.number / 8;
                } else {
                    imm12 = ins->o3.number / 4;
                }
            } else {
                // o3_type == NONE
                imm12 = 0;
            }
            result = (1 << SDT_INDICATOR_SHIFT) | (sf << SF_SHIFT) | (SDT_CONSTANT << SDT_CONSTANT_SHIFT) |
                    (1 << U_SHIFT) | (L << L_SHIFT) | (imm12 << OFFSET_SHIFT) | (xn << XN_SHIFT) | rt;
            break;
        case REGISTER_OFFSET:
            xn = register_number(ins->o2.reg, &xn_64_bit);
            xm = register_number(ins->o3.reg, &xm_64_bit);
            result = (1 << SDT_INDICATOR_SHIFT) | (sf << SF_SHIFT) | (SDT_CONSTANT << SDT_CONSTANT_SHIFT) |
                (L << L_SHIFT) | (1 << (L_SHIFT - 1)) | (xm << XM_SHIFT) | (REG_OFS_CONSTANT << REG_OFS_CONSTANT_SHIFT) | (xn << XN_SHIFT) | rt;
            break;
        case PRE_INDEX:
            xn = register_number(ins->o2.reg, &xn_64_bit);
            simm9 = ins->o3.number; 
            
            result = (1 << SDT_INDICATOR_SHIFT) | (sf << SF_SHIFT) | (SDT_CONSTANT << SDT_CONSTANT_SHIFT) |
                (L << L_SHIFT) | (((simm9 << SIMM9_SHIFT) | ((1 << I_SHIFT) | 1)) << OFFSET_SHIFT) | (xn << XN_SHIFT) | rt;

            break;
        case POST_INDEX:
            xn = register_number(ins->o2.reg, &xn_64_bit);
            simm9 = ins->o3.number;

            result = (1 << SDT_INDICATOR_SHIFT) | (sf << SF_SHIFT) | (SDT_CONSTANT << SDT_CONSTANT_SHIFT) |
                (L << L_SHIFT) | (((simm9 << SIMM9_SHIFT) + 1) << OFFSET_SHIFT) | (xn << XN_SHIFT) | rt;

            break;
        case LOAD_LITERAL:
            if (L == 0) {
                // error
                return EXIT_FAILURE;
            }

            result = (sf << SF_SHIFT) | (LL_CONSTANT << LL_CONSTANT_SHIFT) | ((ins->o2.number & build_mask(0,18)) << SIMM19_SHIFT) | rt;
    }

    return result;
}
