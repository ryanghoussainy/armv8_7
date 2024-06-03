#pragma once
#include <stdio.h>
#include "instr-utils.h"

#define REGISTER_COUNT 31
#define MEM_ADDRESSES 2097152


enum PSTATE_flag {
    N,
    Z,
    C,
    V
};

typedef struct {
    int N;
    int Z;
    int C;
    int V;
} PSTATE_flags;

typedef struct {
    uint8_t memory[MEM_ADDRESSES];
    uint64_t registers[REGISTER_COUNT];
    uint64_t PC;
    uint64_t ZR;
    PSTATE_flags PSTATE;
} CPU;

void initialise_cpu(CPU* cpu);

void write_register(CPU* cpu, int register_no, uint64_t value, int is64_bit);
uint64_t read_register(CPU* cpu, int register_no, int is64_bit);

void write_byte_memory(CPU* cpu, int address, uint8_t byte);
uint8_t read_byte_memory(CPU* cpu, int address);
uint64_t read_bytes_memory(CPU* cpu, int address, int bytes);
uint64_t read_bytes_memory_reverse(CPU* cpu, int start_addr, int bytes);
void write_bytes_memory(CPU* cpu, int address, uint64_t value, int bytes);

void set_flag(CPU* cpu, enum PSTATE_flag flag, int value);

void print_cpu(CPU* cpu, FILE* out);
