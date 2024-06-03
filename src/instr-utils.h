#pragma once
#include <stdint.h>

uint64_t build_mask(int start_bit, int end_bit);
uint32_t parse_ins(uint32_t instr, int start, int end);
uint64_t sign_extend(uint32_t num, int num_bits);
