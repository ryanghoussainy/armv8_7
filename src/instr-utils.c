#include "instr-utils.h"

uint64_t build_mask(int start_bit, int end_bit)
{
    uint64_t mask = 0;

    for (int bit = start_bit; bit <= end_bit; bit++)
    {
        mask |= 1ull << bit;
    }

    return mask;
}

uint32_t parse_ins(uint32_t instr, int start, int end) {
    return (build_mask(start, end) & instr) >> start;
}

uint64_t sign_extend(uint32_t num, int num_bits) {
    // If positive pad with 0's until 64 bits
    // If negative pad with 1's until 64 bits

    int msb = (num >> (num_bits - 1)) & 1;

    if (msb) {

        uint64_t neg_mask = build_mask(num_bits, 63);
        return neg_mask | num;

    } else {

        return (uint64_t) num;
        
    }

    return 0;
}
