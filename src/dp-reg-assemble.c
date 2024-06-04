#include "dp-reg-assemble.h"


static uint32_t build_dp_reg_arithmetic(Instruction* instr) {
    // opc - bits 29-30
    uint32_t opc;
    char* op = instr->operation;
    if (strcmp(op, "add") == 0) {
        opc = 0;
    } else if (strcmp(op, "adds") == 0) {
        opc = 1;
    } else if (strcmp(op, "sub") == 0) {
        opc = 2;
    } else if (strcmp(op, "subs") == 0) {
        opc = 3;
    }

    // rd - bits 0-4
    char* rd_str = instr->o1.reg;
    uint32_t rd = atoi(rd_str + 1);

    // sf - bit 31
    uint32_t sf = rd_str[0] == 'x';

    // rn - bits 5-9
    char* rn_str = instr->o2.reg;
    uint32_t rn = atoi(rn_str + 1);

    // rm - bits 16-20
    char* rm_str = instr->o3.reg;
    uint32_t rm = atoi(rm_str + 1);

    // operand - bits 10-15
    uint32_t operand = instr->o4.number;

    // shift - bits 22-23
    uint32_t shift;
    switch (instr->o4_type) {
        case LSL:
            shift = 0;
            break;
        case LSR:
            shift = 1;
            break;
        case ASR:
            shift = 2;
            break;
        case ROR:
            printf("ROR is invalid shift for arithmetic instruction\n");
            exit(1);
    }

    // opr - bits 21-24
    uint32_t opr = 8 + (shift << 1);

    return (sf << 31) | (opc << 29) | (5 << 25) | (opr << 21) | (rm << 16) | (rn << 5) | rd;
}

static uint32_t build_dp_reg_logical(Instruction* instr) {
    // opc - bits 29-30, N - bit 21
    uint32_t opc, N;
    char* op = instr->operation;
    if (strcmp(op, "and") == 0) {
        opc = 0;
        N = 0;
    } else if (strcmp(op, "bic") == 0) {
        opc = 0;
        N = 1;
    } else if (strcmp(op, "orr") == 0) {
        opc = 1;
        N = 0;
    } else if (strcmp(op, "orn") == 0) {
        opc = 1;
        N = 1;
    } else if (strcmp(op, "eor") == 0) {
        opc = 2;
        N = 0;
    } else if (strcmp(op, "eon") == 0) {
        opc = 2;
        N = 1;
    } else if (strcmp(op, "ands") == 0) {
        opc = 3;
        N = 0;
    } else if (strcmp(op, "bics") == 0) {
        opc = 3;
        N = 1;
    } else {
        printf("Invalid operation for logical instruction\n");
        exit(1);
    }

    // rd - bits 0-4
    char* rd_str = instr->o1.reg;
    uint32_t rd = atoi(rd_str + 1);
    
    // sf - bit 31
    uint32_t sf = rd_str[0] == 'x';

    // rn - bits 5-9
    char* rn_str = instr->o2.reg;
    uint32_t rn = atoi(rn_str + 1);

    // rm - bits 16-20
    char* rm_str = instr->o3.reg;
    uint32_t rm = atoi(rm_str + 1);

    // operand - bits 10-15
    uint32_t operand = instr->o4.number;

    // shift - bits 22-23
    uint32_t shift;
    switch (instr->o4_type) {
        case LSL:
            shift = 0;
            break;
        case LSR:
            shift = 1;
            break;
        case ASR:
            shift = 2;
            break;
        case ROR:
            shift = 3;
            break;
    }

    // opr - bits 21-24
    uint32_t opr = shift << 1 + N;

    return (sf << 31) | (opc << 29) | (5 << 25) | (opr << 21) | (rm << 16) | (operand << 10) | (rn << 5) | rd;
}

uint32_t build_dp_reg_multiply(Instruction* instr) {
    // rd - bits 0-4
    char* rd_str = instr->o1.reg;
    uint32_t rd = atoi(rd_str + 1);

    // sf - bit 31
    uint32_t sf = rd_str[0] == 'x';

    // ra - bits 5-9
    char* rn_str = instr->o2.reg;
    uint32_t rn = atoi(rn_str + 1);

    // rm - bits 16-20
    char* rm_str = instr->o3.reg;
    uint32_t rm = atoi(rm_str + 1);

    // ra - bits 10-14
    char* ra_str = instr-.o4.reg;
    uint32_t ra = atoi(ra_str + 1);

    // x - bit 15
    uint32_t x = strcmp(instr->operation, "msub") == 0;

    return (sf << 31) | (216 << 21) | (rm << 16) | (x << 15) | (ra << 10) | (rn << 5) | rd;
}

uint32_t build_dp_reg(Instruction* instr) {
    char* op = instr->operation;

    if (strstr(op, "add") != NULL || strstr(op, "sub") != NULL) {
        return build_dp_reg_arithmetic(instr);
    } else if (strstr(op, "and") != NULL || strstr(op, "bic") != NULL || strstr(op, "eo") != NULL || strstr(op, "or") != NULL) {
        return build_dp_reg_logical(instr);
    } else if (strstr(op, "madd") != NULL || strstr(op, "msub") != NULL) {
        return build_dp_reg_multiply(instr);
    } else {
        printf("Invalid operation %s for DP register instruction\n", op);
        exit(1);
    }
}
