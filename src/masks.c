#include "masks.h"

uint32_t build_mask(int start_bit, int end_bit)
{
    uint32_t mask = 0;

    for (int bit = start_bit; bit <= end_bit; bit++)
    {
        mask |= 1 << bit;
    }

    return mask;
}