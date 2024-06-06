#include "dp-assemble.h"


uint32_t dp_assembly(Instruction* instr) {
    char* op = instr->operation;

    if (strstr(op, "add") || strstr(op, "sub")) {
        // add(s), sub(s)
        switch (instr->o3_type) {
            case REGISTER:
                return build_dp_reg(instr);
            case LITERAL:
                return build_dp_imm(instr);
            default:
                printf("Third argument of DP must be either REGISTER or LITERAL.\n");
                exit(1);
        }
    } else if (strstr(op, "cm") || strstr(op, "neg")) {
        // cmp, cmn, neg(s)
        switch (instr->o2_type) {
            case REGISTER:
                return build_dp_reg(instr);
            case LITERAL:
                return build_dp_imm(instr);
            default:
                printf("Third argument of DP must be either REGISTER or LITERAL.\n");
                exit(1);
        }
    } else if (strstr(op, "mov") && strcmp(op, "mov") != 0) {
        // movk, movn, movz
        return build_dp_imm(instr);
    } else {
        // and(s), bic(s), eor, orr, eon, orn, tst, mov, mvn, madd, msub, mul, mneg
        return build_dp_reg(instr);
    }
}
