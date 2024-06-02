#include "transfer.h"
#include "branch.h"
#include <stdbool.h>

uint32_t parse_ins(uint32_t instr, int start, int end) {
    return build_mask(start, end) & instr >> start;
}

static uint64_t indexed(CPU* cpu, uint16_t offset, uint8_t xn) {
    int simm9 = parse_ins(offset, 2, 10);
    uint64_t xn_val = read_register(cpu, xn, 1);
    uint64_t address;
    if(parse_ins(offset, 1, 1)) {
        // pre-index
        address = xn_val + simm9;
    } else {
        // post-index 
        address = xn_val;
    }
    write_register(cpu, xn, xn_val + simm9, 1);
    return address;
}

static int single_data(CPU* cpu, uint32_t instr) {
    uint8_t rt = parse_ins(instr, 0, 4);
    bool sf = parse_ins(instr, 30, 30); // 0 is 32-bit, 1 is 64-bit
    bool u = parse_ins(instr, 24, 24);
    bool l = parse_ins(instr, 22, 22);
    uint16_t offset = parse_ins(instr, 10, 21);
    uint8_t xn = parse_ins(instr, 5, 9);
    
    uint64_t base = read_register(cpu, xn, 1);
    uint64_t address;

    if (u) {
        /* 
            unsigned offset
            uoffset = offset * 4 if 32-bit, offset * 8 if 64-bit 
        */
        uint64_t uoffset = offset << (2 + sf);
        address = base + uoffset;
    } else {
        if (parse_ins(instr, 21, 21)) {
            // register offset
            uint8_t xm = parse_ins(instr, 16, 20);
            address = base + read_register(cpu, xm, 1);
        } else {
            // indexed
            address = indexed(cpu, offset, xn);
        }
    }

    if (l) {
        // load
        write_register(cpu, rt, read_bytes_memory_reverse(cpu, address, 4 + (4 * sf)), sf);
    } else {
        // store
        write_bytes_memory(cpu, address, read_register(cpu, rt, sf), 4 + (4 * sf));
    }

    return 1;
}

static int load_literal(CPU* cpu, uint32_t instr) {
    uint8_t rt = parse_ins(instr, 0, 4);
    bool sf = parse_ins(instr, 30, 30);
    uint32_t simm19 = parse_ins(instr, 5, 23);
    uint32_t offset = sign_extend(simm19 * 4, 21);
    uint64_t address = cpu->PC + offset;

    write_register(cpu, rt, read_bytes_memory_reverse(cpu, address, 4 + 4 * sf), sf);
    return 1;
}

int transfer_instruction(CPU* cpu, uint32_t instr){
    if(parse_ins(instr, 31, 31)) {
        // single data transfer
        return single_data(cpu, instr);
    } else {
        // load literal
        return load_literal(cpu, instr);
    }
}
