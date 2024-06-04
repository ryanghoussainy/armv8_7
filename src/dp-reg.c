#include "dp-reg.h"

static DPRegComponents get_dp_reg_components(uint32_t instr)
{
    DPRegComponents components = {
        parse_ins(instr, SF_BITS), // sf
        parse_ins(instr, OPC_BITS), // opc
        parse_ins(instr, M_BITS), // M
        parse_ins(instr, OPR_BITS), // opr
        parse_ins(instr, RM_BITS), // rm
        parse_ins(instr, OPERAND_BITS), // operand
        parse_ins(instr, RN_BITS),   // rn
        parse_ins(instr, RD_BITS),   // rd
        parse_ins(instr, SHIFT_BITS), // shift
        parse_ins(instr, N_BITS), // N
        parse_ins(instr, X_BITS), // x
        parse_ins(instr, RA_BITS)  // ra
    };

    return components;
}

static uint64_t logical_operation(
    CPU* cpu, 
    uint64_t sf, 
    uint64_t opc, 
    uint64_t N_bit, 
    uint64_t Rn, 
    uint64_t op2
) {
    uint64_t result;
    switch (N_bit) {
        case 0:
            switch (opc) {
                case 0: // Bitwise AND
                    return Rn & op2;
                case 1: // Bitwise inclusive OR
                    return Rn | op2;
                case 2: // Bitwise exclusive OR
                    return Rn ^ op2;
                case 3: // Bitwise AND, setting flags
                    result = Rn & op2;
                    // N flag
                    set_flag(cpu, N, sf ? result >> (num_bits(sf)-1) : (result & UINT32_MAX) >> (num_bits(sf)-1));
                    // Z flag
                    set_flag(cpu, Z, result == 0);
                    // C flag
                    set_flag(cpu, C, 0);
                    // V flag
                    set_flag(cpu, V, 0);
                    
                    return result;
                default:
                    printf("Error occured with value of opc\n");
                    assert(0);
            }
        case 1:
            switch (opc) {
                case 0: // Bitwise bit clear
                    return Rn & ~op2;
                case 1: // Bitwise inclusive OR NOT
                    return Rn | ~op2;
                case 2: // Bitwise exclusive OR NOT
                    return Rn ^ ~op2;
                case 3: // Bitwise bit clear, setting flags 
                    result = Rn & ~op2;
                    // N flag
                    set_flag(cpu, N, sf ? result >> (num_bits(sf)-1) : (result & UINT32_MAX) >> (num_bits(sf)-1));
                    // Z flag
                    set_flag(cpu, Z, result == 0);
                    // C flag
                    set_flag(cpu, C, 0);
                    // V flag
                    set_flag(cpu, V, 0);
                    
                    return result;
                default:
                    printf("Error occured with value of opc\n");
                    assert(0);
            }
        default:
            printf("Error occured with value of N\n");
            assert(0);
    }
}

static uint64_t multiply_operation(
    CPU* cpu,
    uint64_t x,
    uint64_t Ra,
    uint64_t Rn,
    uint64_t Rm
) {
    switch (x) {
        case 0: // Multiply-Add
            return Ra + Rn * Rm;
        case 1: // Multiply-Sub
            return Ra - Rn * Rm;
        default:
            printf("Error occured with value of x\n");
            assert(0);
    }
}

static uint64_t perform_shift(uint64_t sf, uint64_t shift, uint64_t Rm, uint64_t operand) {
    switch (shift) {
        case 0:
            return Rm << operand;
        case 1:
            return Rm >> operand;
        case 2: // Casting to the signed version and shifting does a sign extension
            if (sf) {
                return (int64_t)Rm >> operand;
            } else {
                return (int32_t)Rm >> operand;
            }
        case 3:
            return (Rm >> operand) | (Rm << (num_bits(sf) - operand));
        default:
            printf("Error occured with value of shift\n");
            assert(0);
    }
}

static int reg_arithmetic(CPU* cpu, DPRegComponents* components) {
    if (components->shift == 3) {
        printf("Invalid instruction\n");
        return 0;
    }
    uint64_t Rm = read_register(cpu, components->rm, components->sf);
    uint64_t op2 = perform_shift(components->sf, components->shift, Rm, components->operand);
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rd = arithmetic_operation(cpu, components->sf, components->opc, Rn, op2);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

static int reg_logical(CPU* cpu, DPRegComponents* components) {
    uint64_t Rm = read_register(cpu, components->rm, components->sf);
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t op2 = perform_shift(components->sf, components->shift, Rm, components->operand);
    uint64_t Rd = logical_operation(cpu, components->sf, components->opc, components->N, Rn, op2);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

static int reg_multiply(CPU* cpu, DPRegComponents* components) {
    uint64_t Ra = read_register(cpu, components->ra, components->sf);
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rm = read_register(cpu, components->rm, components->sf);
    uint64_t Rd = multiply_operation(cpu, components->x, Ra, Rn, Rm);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

int dp_reg_instruction(CPU* cpu, uint32_t instr) {
    DPRegComponents components = get_dp_reg_components(instr);

    if (components.M == 0) {
        if (components.opr >= 8 && components.opr % 2 == 0) {
            return reg_arithmetic(cpu, &components);
        } else if (components.opr < 8) {
            return reg_logical(cpu, &components);
        } else {
            printf("Invalid instruction\n");
            return 0;
        }
    } else if (components.opr == 8 && components.opc == 0) {
        return reg_multiply(cpu, &components);
    } else {
        printf("Invalid instruction\n");
        return 0;
    }
}
