#include "dp-reg.h"
#include "masks.h"
#include <assert.h>

DPRegComponents get_dp_reg_components(uint32_t instr)
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

    DPRegComponents components = {
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

int dp_reg_instruction(CPU* cpu, uint32_t instr) {
    DPRegComponents components = get_dp_reg_components(instr);

    if (components.M == 0) {
        if (components.opr >= 8 && components.opr % 2 == 0) {
            return reg_arithmetic(cpu, &components);
        } else if (components.opr < 8) {

            if (instr == 2315255808) {
                // Halting condition
                return 0;
            }
            
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

int reg_arithmetic(CPU* cpu, DPRegComponents* components) {
    if (components->shift == 3) {
        printf("Invalid instruction\n");
        return 0;
    }
    uint64_t op2 = perform_shift(components->shift, components->rm, components->operand);
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rd = arithmetic_operation(cpu, components->sf, components->opc, Rn, op2);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

uint64_t arithmetic_operation(
    CPU* cpu,
    uint64_t sf, 
    uint64_t opc,
    uint64_t Rn, 
    uint64_t op2
) {
    uint64_t result, msb_result, msb_Rn, msb_op2;
    
    switch (opc) {
        case 0: // Add
            return Rn + op2;
        case 1: // Add, set flags
            result = Rn + op2;

            msb_result = sf ? result >> 63 : result >> 31;
            msb_Rn = sf ? Rn >> 63 : Rn >> 31;
            msb_op2 = sf ? op2 >> 63 : op2 >> 31;

            // N flag
            set_flag(cpu, N, msb_result);
            // Z flag
            set_flag(cpu, Z, result == 0);
            // C flag
            if (sf) {
                set_flag(cpu, C, result < Rn || result < op2);
            } else {
                set_flag(cpu, C, result > INT32_MAX);
            }
            // V flag
            set_flag(cpu, V, msb_Rn == msb_op2 && msb_Rn != msb_result);

            return result;
        case 2: // Subtract
            return Rn - op2;
        case 3: // Subtract, set flags
            result = Rn - op2;

            msb_result = sf ? result >> 63 : result >> 31;
            msb_Rn = sf ? Rn >> 63 : Rn >> 31;
            msb_op2 = sf ? op2 >> 63 : op2 >> 31;

            // N flag
            set_flag(cpu, N, msb_result);
            // Z flag
            set_flag(cpu, Z, result == 0);
            // C flag
            set_flag(cpu, C, op2 > Rn);
            // V flag
            set_flag(cpu, V, msb_Rn == msb_op2 && msb_Rn != msb_result);

            return result;
        default:
            printf("Error occured with value of opc\n");
            assert(0);
    }
}

int reg_logical(CPU* cpu, DPRegComponents* components) {
    uint64_t op2 = perform_shift(components->shift, components->rm, components->operand);
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rd = logical_operation(cpu, components->sf, components->opc, components->N, Rn, op2);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

uint64_t logical_operation(
    CPU* cpu, 
    uint64_t sf, 
    uint64_t opc, 
    uint64_t N, 
    uint64_t Rn, 
    uint64_t op2
) {
    uint64_t result;
    switch (N) {
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
                    set_flag(cpu, N, sf ? result >> 63 : result >> 31);
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
                    set_flag(cpu, N, sf ? result >> 63 : result >> 31);
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

int reg_multiply(CPU* cpu, DPRegComponents* components) {
    uint64_t Ra = read_register(cpu, components->ra, components->sf);
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rm = read_register(cpu, components->rm, components->sf);
    uint64_t Rd = multiply_operation(cpu, components->x, Ra, Rn, Rm);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

uint64_t multiply_operation(
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

uint64_t perform_shift(uint64_t shift, uint64_t rm, uint64_t operand) {
    switch (shift) {
        case 0:
            return rm << operand;
        case 1:
            return rm >> operand;
        case 2:
            return (int)rm >> operand; // Casting to int does a sign extension
        case 3:
            return (rm >> operand) | (rm << (32 - operand));
        default:
            printf("Error occured with value of shift\n");
            assert(0);
    }
}
