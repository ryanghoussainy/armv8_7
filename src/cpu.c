#include "cpu.h"

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

void write_register(struct CPU* cpu, int register_no, int64_t value)
{
    cpu->registers[register_no] = value;
}

int64_t read_register(struct CPU* cpu, int register_no)
{
    return cpu->registers[register_no];
}

void write_byte_memory(struct CPU* cpu, int address, int8_t byte)
{
    cpu->memory[address] = byte;
}

int8_t read_byte_memory(struct CPU* cpu, int address)
{
    return cpu->memory[address];
}

int64_t read_bytes_memory(struct CPU* cpu, int start_addr, int bytes)
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
        
        int64_t reg_value = read_register(cpu, register_no);
        fprintf(out, "%016x\n", reg_value);
    }

    fprintf(out, "PC = %016x\n", cpu->PC);
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
        int64_t mem_value = read_bytes_memory(cpu, address, 4);

        if (mem_value == 0)
            continue;
        
        fprintf(out, "0x%08x = ", address);
        fprintf(out, "0x%08x\n", mem_value);
    }
}

void print_cpu(struct CPU* cpu, FILE* out)
{
    print_registers(cpu, out);
    print_pstate(cpu, out);
    print_non_zero_memory(cpu, out);
}