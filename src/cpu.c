#include "cpu.h"
#include "masks.h"

void initialise_cpu(struct CPU* cpu)
{
    for (int register_no = 0; register_no < REGISTER_COUNT; register_no++) {
        cpu->registers[register_no] = 0;
    }

    for (int address = 0; address < MEM_ADDRESSES; address++) {
        cpu->memory[address] = 0;
    }

    cpu->PC = 0;

    cpu->PSTATE.N = 0;
    cpu->PSTATE.Z = 1;
    cpu->PSTATE.C = 0;
    cpu->PSTATE.V = 0;

    cpu->ZR = 0;
}

void write_register(struct CPU* cpu, int register_no, uint64_t value, int is_64bit)
{
    if (register_no == 31)
        return;
        
    if (!is_64bit) {
        uint64_t _32bit_mask = build_mask(0, 31);
        uint64_t inverse_mask = ~_32bit_mask;
        uint64_t register_value = read_register(cpu, register_no, 1);
        uint64_t top_bits = inverse_mask & register_value;
        uint64_t _32bit_mask = build_mask(0, 31);

        cpu->registers[register_no] = top_bits | (value & _32bit_mask);
    }else {
        cpu->registers[register_no] = value;
    }
}

uint64_t read_register(struct CPU* cpu, int register_no, int is_64bit)
{
    if (register_no == 31)
        return 0;

    uint64_t register_value = cpu->registers[register_no];

    if (!is_64bit) {
        uint64_t _32bit_mask = build_mask(0, 31);
        return register_value & _32bit_mask;
    }

    return register_value;
}

void write_byte_memory(struct CPU* cpu, int address, uint8_t byte)
{
    cpu->memory[address] = byte;
}

void write_bytes_memory(struct CPU* cpu, int address, uint64_t value, int bytes) {
    uint64_t mask = 255;  // set lowest 8 bits all to 1
    for (int byte = 0; byte < bytes; byte++) {
        mask <<= (byte * 8);
        uint8_t current_byte = value & mask >> (byte * 8);
        write_byte_memory(cpu, address + byte, current_byte);
    }
}

uint8_t read_byte_memory(struct CPU* cpu, int address)
{
    return cpu->memory[address];
}

uint64_t read_bytes_memory(struct CPU* cpu, int start_addr, int bytes)
{
    int64_t result = 0;
    for (int byte = 0; byte < bytes; byte++) {
        int64_t curr_byte = read_byte_memory(cpu, start_addr + byte);
        result = (result << 8) | curr_byte;
    }

    return result;
}

void set_flag(struct CPU* cpu, enum PSTATE_flag flag, int value)
{
    switch(flag) {
        case N:
            cpu->PSTATE.N = value;
            break;
        case Z:
            cpu->PSTATE.Z = value;
            break;
        case C:
            cpu->PSTATE.C = value;
            break;
        case V:
            cpu->PSTATE.V = value;
            break;
    }
}


void print_registers(struct CPU* cpu, FILE* out)
{
    fprintf(out, "Registers: \n");

    for (int register_no = 0; register_no < REGISTER_COUNT; register_no++) {
        fprintf(out, "X%02d = ", register_no);
        
        uint64_t reg_value = read_register(cpu, register_no, 1);
        fprintf(out, "%016lx\n", reg_value);
    }

    fprintf(out, "PC = %016lx\n", cpu->PC);
}

void print_pstate(struct CPU* cpu, FILE* out)
{
    fprintf(out, "PSTATE : ");

    if (cpu->PSTATE.N > 0) {
        fprintf(out, "N");
    }else {
        fprintf(out, "-");
    }

    if (cpu->PSTATE.Z > 0) {
        fprintf(out, "Z");
    }else {
        fprintf(out, "-");
    }

    if (cpu->PSTATE.C > 0) {
        fprintf(out, "C");
    }else {
        fprintf(out, "-");
    }

    if (cpu->PSTATE.V > 0) {
        fprintf(out, "V");
    }else {
        fprintf(out, "-");
    }

    fprintf(out, "\n");
}

void print_non_zero_memory(struct CPU* cpu, FILE* out)
{
    fprintf(out, "Non-zero memory: \n");

    for (int address = 0; address < MEM_ADDRESSES; address += 4) {
        uint64_t mem_value = read_bytes_memory(cpu, address, 4);

        if (mem_value == 0)
            continue;
        
        fprintf(out, "0x%08x = ", address);
        fprintf(out, "0x%08lx\n", mem_value);
    }
}

void print_cpu(struct CPU* cpu, FILE* out)
{
    print_registers(cpu, out);
    print_pstate(cpu, out);
    print_non_zero_memory(cpu, out);
}