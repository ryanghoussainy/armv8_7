#pragma once
#include <assert.h>
#include "cpu.h"

uint64_t arithmetic_operation(
    CPU* cpu,
    uint64_t sf, 
    uint64_t opc,
    uint64_t Rn, 
    uint64_t op2
);
