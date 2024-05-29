#include "execute.h"
#include "dp-imm.h"
#include "transfer.h"
#include "branch.h"

#define OP0_BITS 4

int execute_instruction(struct CPU* cpu, uint32_t instruction)
{
    uint64_t op0 = (make_mask(25, 28) & instruction) >> 25;
    uint64_t op0_bits[OP0_BITS];

    for (int bit = OP0_BITS - 1; bit >= 0; bit--) {
        uint64_t mask = mask_mask(bit, bit);
        uint64_t bit_value = (op0 & mask) >> bit;
        op0_bits[bit] = bit_value;
    }

    if (op0_bits[0] == 1 && op0_bits[1] == 0 && op0_bits[2] == 0) {
        dp_imm_instruction(cpu, instruction);
    }else if (op0_bits[1] == 1 && op0_bits[2] == 0 && op0_bits[3] == 1) {
        // dp register instruction
    }else if (op0_bits[1] == 1 && op0_bits[3] == 0) {
        single_data(cpu, instruction);
    }else if (op0_bits[0] == 1 && op0_bits[1] == 0 && op0_bits[2] == 1) {
        branch_instruction(cpu, instruction);
    }else {
        printf("Invalid instruction\n");
        return 0;
    }

    return 1;
}
