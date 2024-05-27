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
    int8_t memory[MEM_ADDRESSES];
    int64_t registers[REGISTER_COUNT];
    int64_t PC;
    int64_t ZR;
    struct PSTATE_flags PSTATE;
};

void initialise_cpu(struct CPU* cpu);

void write_register(struct CPU* cpu, int register_no, int64_t value);
int64_t read_register(struct CPU* cpu, int register_no);

void write_byte_memory(struct CPU* cpu, int address, int8_t byte);
int8_t read_byte_memory(struct CPU* cpu, int address);
int64_t read_bytes_memory(struct CPU* cpu, int address, int bytes);

void set_flag(struct CPU* cpu, enum PSTATE_flag flag, int value);

void print_cpu(struct CPU* cpu, FILE* out);