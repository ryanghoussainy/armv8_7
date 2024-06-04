#include "arithmetic-op.h"

uint64_t arithmetic_operation(
    CPU* cpu,
    uint64_t sf, 
    uint64_t opc,
    uint64_t Rn, 
    uint64_t op2
) {
    uint64_t result, msb_result, msb_Rn, msb_op2;
    
    if (!sf) {
        Rn = UINT32_MAX & Rn;
        op2 = UINT32_MAX & op2;
    }
    
    switch (opc) {
        case 0: // Add
            return Rn + op2;
        case 1: // Add, set flags
            result = Rn + op2;

            msb_result = sf ? result >> (num_bits(sf)-1) : (result & UINT32_MAX) >> (num_bits(sf)-1);
            msb_Rn = sf ? Rn >> (num_bits(sf)-1) : Rn >> (num_bits(sf)-1);
            msb_op2 = sf ? op2 >> (num_bits(sf)-1) : op2 >> (num_bits(sf)-1);

            // N flag
            set_flag(cpu, N, msb_result);
            // Z flag
            set_flag(cpu, Z, result == 0);
            // C flag
            set_flag(cpu, C, sf ? result < Rn || result < op2 : result > UINT32_MAX);
            // V flag
            set_flag(cpu, V, msb_Rn == msb_op2 && msb_Rn != msb_result);

            return sf ? result : result & UINT32_MAX;
        case 2: // Subtract
            return Rn - op2;
        case 3: // Subtract, set flags
            result = Rn - op2;
            
            msb_result = sf ? result >> (num_bits(sf)-1) : (result & UINT32_MAX) >> (num_bits(sf)-1);
            msb_Rn = sf ? Rn >> (num_bits(sf)-1) : Rn >> (num_bits(sf)-1);
            msb_op2 = sf ? op2 >> (num_bits(sf)-1) : op2 >> (num_bits(sf)-1);

            // N flag
            set_flag(cpu, N, msb_result);
            // Z flag
            set_flag(cpu, Z, result == 0);
            // C flag
            int carry = op2 > Rn;
            set_flag(cpu, C, !carry);
            // V flag
            set_flag(cpu, V, (msb_Rn && !msb_op2 && !msb_result) || (!msb_Rn && msb_op2 && msb_result));

            return result;
        default:
            printf("Error occured with value of opc\n");
            assert(0);
    }
}
