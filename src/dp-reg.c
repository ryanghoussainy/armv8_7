#include "dp-reg.h"
#include "masks.h"

struct DPRegComponents get_components(uint32_t instr)
{
    uint64_t sf_mask = build_mask(31, 31);
    uint64_t opc_mask = build_mask(29, 30);
    uint64_t M_mask = build_mask(28, 28);
    uint64_t opr_mask = build_mask(21, 24);
    uint64_t rm_mask = build_mask(16, 20);
    uint64_t operand_mask = build_mask(10, 15);
    uint64_t rn_mask = build_mask(5, 9);
    uint64_t rd_mask = build_mask(0, 4);
    uint64_t shift_mask = build_mask(22, 23);
    uint64_t N_mask = build_mask(21, 21);
    uint64_t x_mask = build_mask(15, 15);
    uint64_t ra_mask = build_mask(10, 14);

    uint64_t sf = (instr & sf_mask) >> 31;
    uint64_t opc = (instr & opc_mask) >> 29;
    uint64_t M = (instr & M_mask) >> 28;
    uint64_t opr = (instr & opr_mask) >> 21;
    uint64_t rm = (instr & rm_mask) >> 16;
    uint64_t operand = (instr & operand_mask) >> 10;
    uint64_t rn = (instr & rn_mask) >> 5;
    uint64_t rd = (instr & rd_mask);
    uint64_t shift = (instr & shift_mask) >> 22;
    uint64_t N = (instr & N_mask) >> 21;
    uint64_t x = (instr & x_mask) >> 15;
    uint64_t ra = (instr & ra_mask) >> 10;

    struct DPRegComponents components = {
        sf,
        opc,
        M,
        opr,
        rm,
        operand,
        rn,
        rd,
        shift,
        N,
        x,
        ra
    };

    return components;
}

void dp_reg_instruction(struct CPU* cpu, uint32_t instr) {
    struct DPRegComponents components = get_components(instr);

    if (components.M == 0) {
        if (components.opr >= 8 && components.opr % 2) {
            reg_arithmetic(cpu, &components);
        } else if (components.opr < 8) {
            reg_logical(cpu, &components);
        } else {
            // Invalid instruction
        }
    } else if (components.M == 1) {
        reg_multiply(cpu, &components);
    } else {
        // Invalid instruction
    }
}

uint32_t perform_shift(uint32_t shift, uint32_t rm, uint32_t operand) {
    switch (shift) {
        case 0:
            return lsl(rm, operand);
        case 1:
            return lsr(rm, operand);
        case 2:
            return asr(rm, operand);
        case 3:
            return ror(rm, operand);
        default:
            // Error with shift value
    }
}

uint32_t lsl(uint32_t val, uint32_t shift_amount) {
    return val << shift_amount;
}

uint32_t lsr(uint32_t val, uint32_t shift_amount) {
    return val >> shift_amount;
}

uint32_t asr(uint32_t val, uint32_t shift_amount) {
    return (int)val >> shift_amount;
}

uint32_t ror(uint32_t val, uint32_t shift_amount) {
    return (val >> shift_amount) | (val << (32 - shift_amount));
}
