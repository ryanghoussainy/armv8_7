#include "dp-reg-assemble.h"


static uint32_t build_dp_reg_arithmetic(Instruction instr) {
    // opc - bits 29-30
    uint32_t opc;
    char* operation = instr.operation;

    if (strcmp(operation, "add") == 0) {
        opc = 0;
    } else if (strcmp(operation, "adds") == 0) {
        opc = 1;
    } else if (strcmp(operation, "sub") == 0) {
        opc = 2;
    } else if (strcmp(operation, "subs") == 0) {
        opc = 3;
    }

    char* rd_str = instr.o1.reg;
    // sf - bit 31
    uint32_t sf = rd_str[0] == 'x';
    // rd - bits 0-4
    uint32_t rd = atoi(rd_str + 1);

    char* rn_str = instr.o2.reg;
    // rn - bits 5-9
    uint32_t rn = atoi(rn_str + 1);

    char* rm_str = instr.o3.reg;
    // rm - bits 16-20
    uint32_t rm = atoi(rm_str + 1);

    // operand - bits 10-15
    uint32_t operand = instr.o4.number;
    // shift - bits 22-23
    uint32_t shift = 0;
    switch (instr.o4_type) {
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

    return (sf << 31) | (opc << 29) | (13 << 25) | (opr << 21) | (rm << 16) | (rn << 5) | rd;
}

uint32_t build_dp_reg(Instruction instr) {
    char* operation = instr.operation;

    if (strstr(operation, "add") != NULL || strstr(operation, "sub") != NULL) {
        return build_dp_reg_arithmetic(instr);
    } else {
        // Other instructions
    }
}
