#include "dp-reg-assemble.h"


static uint32_t build_dp_reg_arithmetic(Instruction* instr) {
    // opc - bits 29-30
    uint32_t opc;
    char* op = instr->operation;
    if (strcmp(op, "add") == 0) {
        opc = OPC_ADD;
    } else if (strcmp(op, "adds") == 0) {
        opc = OPC_ADDS;
    } else if (strcmp(op, "sub") == 0) {
        opc = OPC_SUB;
    } else if (strcmp(op, "subs") == 0) {
        opc = OPC_SUBS;
    }

    // sf - bit 31
    bool sf;

    // rd - bits 0-4
    char* rd_str = instr->o1.reg;
    uint32_t rd = register_number(rd_str, &sf);

    // rn - bits 5-9
    char* rn_str = instr->o2.reg;
    uint32_t rn = register_number(rn_str, &sf);

    // rm - bits 16-20
    char* rm_str = instr->o3.reg;
    uint32_t rm = register_number(rm_str, &sf);

    // operand - bits 10-15
    uint32_t operand = instr->o4.number;

    // shift - bits 22-23
    uint32_t shift;
    switch (instr->o4_type) {
        case LSL:
            shift = SHIFT_LSL;
            break;
        case LSR:
            shift = SHIFT_LSR;
            break;
        case ASR:
            shift = SHIFT_ASR;
            break;
        case ROR:
            printf("ROR is invalid shift for arithmetic instruction\n");
            exit(1);
        case NONE:
            shift = NO_SHIFT;
            operand = NO_SHIFT;
            break;
        default:
            printf("Error operand");
            exit(1);
    }

    // opr - bits 21-24
    uint32_t opr = ARITH_OPR;

    return (sf << SF_SHIFT) | (opc << OPC_SHIFT) | ARITH_MIDDLE_BITS | (opr << OPR_SHIFT) | (rm << RM_SHIFT) | (operand << OPERAND_SHIFT) | (rn << RN_SHIFT) | rd;
}

static uint32_t build_dp_reg_logical(Instruction* instr) {
    // opc - bits 29-30, N - bit 21
    uint32_t opc, N;
    char* op = instr->operation;
    if (strcmp(op, "and") == 0) {
        opc = OPC_AND;
        N = N_AND;
    } else if (strcmp(op, "bic") == 0) {
        opc = OPC_BIC;
        N = N_BIC;
    } else if (strcmp(op, "orr") == 0) {
        opc = OPC_ORR;
        N = N_ORR;
    } else if (strcmp(op, "orn") == 0) {
        opc = OPC_ORN;
        N = N_ORN;
    } else if (strcmp(op, "eor") == 0) {
        opc = OPC_EOR;
        N = N_EOR;
    } else if (strcmp(op, "eon") == 0) {
        opc = OPC_EON;
        N = N_EON;
    } else if (strcmp(op, "ands") == 0) {
        opc = OPC_ANDS;
        N = N_ANDS;
    } else if (strcmp(op, "bics") == 0) {
        opc = OPC_BICS;
        N = N_BICS;
    } else {
        printf("Invalid operation for logical instruction\n");
        exit(1);
    }

    // sf - bit 31
    bool sf;

    // rd - bits 0-4
    char* rd_str = instr->o1.reg;
    uint32_t rd = register_number(rd_str, &sf);

    // rn - bits 5-9
    char* rn_str = instr->o2.reg;
    uint32_t rn = register_number(rn_str, &sf);

    // rm - bits 16-20
    char* rm_str = instr->o3.reg;
    uint32_t rm = register_number(rm_str, &sf);

    // operand - bits 10-15
    uint32_t operand = instr->o4.number;

    // shift - bits 22-23
    uint32_t shift;
    switch (instr->o4_type) {
        case LSL:
            shift = SHIFT_LSL;
            break;
        case LSR:
            shift = SHIFT_LSR;
            break;
        case ASR:
            shift = SHIFT_ASR;
            break;
        case ROR:
            shift = SHIFT_ROR;
            break;
        case NONE:
            shift = NO_SHIFT;
            operand = NO_SHIFT;
            break;
        default:
            printf("Error operand");
            exit(1);
    }

    // opr - bits 21-24
    uint32_t opr = LOGICAL_OPR;

    return (sf << SF_SHIFT) | (opc << OPC_SHIFT) | LOGICAL_MIDDLE_BITS | (opr << OPR_SHIFT) | (rm << RM_SHIFT) | (operand << OPERAND_SHIFT) | (rn << RN_SHIFT) | rd;
}

static uint32_t build_dp_reg_multiply(Instruction* instr) {
    // sf - bit 31
    bool sf;

    // rd - bits 0-4
    char* rd_str = instr->o1.reg;
    uint32_t rd = register_number(rd_str, &sf);

    // ra - bits 5-9
    char* rn_str = instr->o2.reg;
    uint32_t rn = register_number(rn_str, &sf);

    // rm - bits 16-20
    char* rm_str = instr->o3.reg;
    uint32_t rm = register_number(rm_str, &sf);

    // ra - bits 10-14
    char* ra_str = instr->o4.reg;
    uint32_t ra = register_number(ra_str, &sf);

    // x - bit 15
    uint32_t x = strcmp(instr->operation, "msub") == 0;

    return (sf << SF_SHIFT) | MULTIPLY_MIDDLE_BITS | (rm << RM_SHIFT) | (x << X_SHIFT) | (ra << RA_SHIFT) | (rn << RN_SHIFT) | rd;
}

uint32_t build_dp_reg(Instruction* instr) {
    char* op = instr->operation;

    if (strstr(op, "madd") != NULL || strstr(op, "msub") != NULL) {
        return build_dp_reg_multiply(instr);
    } else if (strstr(op, "add") != NULL || strstr(op, "sub") != NULL) {
        return build_dp_reg_arithmetic(instr);
    } else if (strstr(op, "and") != NULL || strstr(op, "bic") != NULL || strstr(op, "eo") != NULL || strstr(op, "or") != NULL) {
        return build_dp_reg_logical(instr);
    } else {
        printf("Invalid operation %s for DP register instruction\n", op);
        exit(1);
    }
}
