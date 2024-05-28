#pragma once
#include <stdint.h>
#include <stdio.h>

#define REGISTER_COUNT 31
#define MEM_ADDRESSES 2097152


enum PSTATE_flag {
    N,
    Z,
    C,
    V
};

struct PSTATE_flags {
    int N;
    int Z;
    int C;
    int V;
};

struct CPU {
    uint8_t memory[MEM_ADDRESSES];
    uint64_t registers[REGISTER_COUNT];
    uint64_t PC;
    uint64_t ZR;
    struct PSTATE_flags PSTATE;
};

void initialise_cpu(struct CPU* cpu);

void write_register(struct CPU* cpu, int register_no, uint64_t value, int is64_bit);
uint64_t read_register(struct CPU* cpu, int register_no, int is64_bit);

void write_byte_memory(struct CPU* cpu, int address, uint8_t byte);
uint8_t read_byte_memory(struct CPU* cpu, int address);
uint64_t read_bytes_memory(struct CPU* cpu, int address, int bytes);

void set_flag(struct CPU* cpu, enum PSTATE_flag flag, int value);

void print_cpu(struct CPU* cpu, FILE* out);