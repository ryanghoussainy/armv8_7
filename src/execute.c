#include "execute.h"
#include "dp-imm.h"
#include "transfer.h"
#include "branch.h"
#include "masks.h"
#include "dp-reg.h"

#define OP0_BITS 4

int execute_instruction(struct CPU* cpu, uint32_t instruction)
{

    uint64_t op0 = (build_mask(25, 28) & instruction) >> 25;
    uint64_t op0_bits[OP0_BITS];

    for (int bit = OP0_BITS - 1; bit >= 0; bit--) {
        uint64_t mask = build_mask(bit, bit);
        uint64_t bit_value = (op0 & mask) >> bit;
        op0_bits[OP0_BITS - bit - 1] = bit_value;
    }

    if (op0_bits[0] == 1 && op0_bits[1] == 0 && op0_bits[2] == 0) {
        int ret = dp_imm_instruction(cpu, instruction);
        cpu->PC += 4;
        return ret;

    } else if (op0_bits[1] == 1 && op0_bits[2] == 0 && op0_bits[3] == 1) {
        int ret = dp_reg_instruction(cpu, instruction);
        cpu->PC += 4;
        return ret;
    } else if (op0_bits[1] == 1 && op0_bits[3] == 0) {
        int ret = transfer_instruction(cpu, instruction);
        cpu->PC += 4;
        return ret;

    } else if (op0_bits[0] == 1 && op0_bits[1] == 0 && op0_bits[2] == 1) {
        return branch_instruction(cpu, instruction);
    }

    printf("Invalid instruction\n");
    return 0;
}

uint32_t flip_endian(uint32_t instruction) {

    // need to flip instruction to little endian
    // chaos below is my attempt to do that

    uint32_t bit_num = 2147483648; // Value of the 32nd bit

    uint32_t flipped = (instruction & bit_num) >> 31;

    for (int i = 0; i < 30; i++) {
        bit_num = bit_num / 2;

        if (i <= 14) {
            flipped += (instruction & bit_num) >> (29 - (2 * i));
        } else {
            flipped += (instruction & bit_num) << ((2 * (i - 15)) + 1);
        }
    }
    
    bit_num = bit_num / 2;
    
    flipped += (instruction & bit_num) << 31;

    return flipped;
}

void cycle(struct CPU* cpu) {

  int is_running = 1;

  while (is_running) {
    uint32_t instruction = flip_endian(read_bytes_memory(cpu, cpu->PC, 4));

    printf("Current instruction: %u\n",instruction);

    if (instruction == 0) { // For testing purposes terminate if 0 instruction
        is_running = 0;
    } else {
        is_running = execute_instruction(cpu, instruction);
    }
  }
}
