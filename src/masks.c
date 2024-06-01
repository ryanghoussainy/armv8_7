#include "masks.h"

uint64_t build_mask(int start_bit, int end_bit)
{
    uint64_t mask = 0;

    for (int bit = start_bit; bit <= end_bit; bit++)
    {
        mask |= 1ull << bit;
    }

    return mask;
}
