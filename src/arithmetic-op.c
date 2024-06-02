#include "arithmetic-op.h"

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
            
            msb_result = sf ? result >> 63 : (result & UINT32_MAX) >> 31;
            msb_Rn = sf ? Rn >> 63 : Rn >> 31;
            msb_op2 = sf ? op2 >> 63 : op2 >> 31;

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

            msb_result = sf ? result >> 63 : (result & UINT32_MAX) >> 31;
            msb_Rn = sf ? Rn >> 63 : Rn >> 31;
            msb_op2 = sf ? op2 >> 63 : op2 >> 31;

            // N flag
            set_flag(cpu, N, msb_result);
            // Z flag
            set_flag(cpu, Z, result == 0);
            // C flag
            set_flag(cpu, C, op2 <= Rn);
            // V flag
            set_flag(cpu, V, (msb_Rn && !msb_op2 && !msb_result) || (!msb_Rn && msb_op2 && msb_result));

            return result;
        default:
            printf("Error occured with value of opc\n");
            assert(0);
    }
}
