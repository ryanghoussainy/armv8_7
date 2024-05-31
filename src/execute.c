#include "execute.h"
#include "dp-imm.h"
#include "transfer.h"
#include "branch.h"
#include "masks.h"
#include "dp-reg.h"

#define OP0_BITS 4

int execute_instruction(CPU* cpu, uint32_t instruction)
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

void cycle(CPU* cpu) {

  int isRunning = 1;

  while (isRunning) {
    uint32_t instruction = read_bytes_memory(cpu, cpu->PC, 4);
    isRunning = execute_instruction(cpu, instruction);
  }
}
