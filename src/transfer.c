#include "transfer.h"

static uint64_t indexed(CPU* cpu, uint16_t offset, uint8_t xn) {
    signed int simm9 = sign_extend(parse_ins(offset, OFFSET_SIMM9_BITS), SIMM9_NUM_BITS);

    uint64_t xn_val = read_register(cpu, xn, 1);
    uint64_t address;
    if(parse_ins(offset, OFFSET_I_BITS)) {
        // pre-index
        address = xn_val + simm9;
        write_register(cpu, xn, address, 1);
    } else {
        // post-index 
        address = xn_val;
        write_register(cpu, xn, xn_val + simm9, 1);
    }
    
    return address;
}

static int single_data(CPU* cpu, uint32_t instr) {
    uint8_t rt = parse_ins(instr, INSTR_RT_BITS);
    bool sf = parse_ins(instr, INSTR_SF_BITS); // 0 is 32-bit, 1 is 64-bit
    bool u = parse_ins(instr, INSTR_U_BITS);
    bool l = parse_ins(instr, INSTR_L_BITS);
    uint16_t offset = parse_ins(instr, INSTR_OFFSET_BITS);
    uint8_t xn = parse_ins(instr, INSTR_XN_BITS);
    
    uint64_t base = read_register(cpu, xn, 1);
    uint64_t address;

    if (u) {
        /* 
            unsigned offset
            uoffset = offset * 4 if 32-bit, offset * 8 if 64-bit 
        */
        uint64_t uoffset = offset << (UOFFSET_SHIFT + sf);
        address = base + uoffset;
    } else {
        if (parse_ins(instr, INSTR_TOP_OFFSET_BITS)) {
            // register offset
            uint8_t xm = parse_ins(instr, INSTR_XM_BITS);
            address = base + read_register(cpu, xm, 1);
        } else {
            // indexed
            address = indexed(cpu, offset, xn);
        }
    }

    if (l) {
        // load
        write_register(cpu, rt, read_bytes_memory_reverse(cpu, address, NUM_BYTES + (NUM_BYTES * sf)), sf);
    } else {
        // store
        write_bytes_memory(cpu, address, read_register(cpu, rt, sf), NUM_BYTES + (NUM_BYTES * sf));
    }

    return 1;
}

static int load_literal(CPU* cpu, uint32_t instr) {
    uint8_t rt = parse_ins(instr, INSTR_RT_BITS);
    bool sf = parse_ins(instr, INSTR_SF_BITS);
    uint32_t simm19 = parse_ins(instr, INSTR_SIMM19_BITS);
    uint32_t offset = sign_extend(simm19 << TRANSFER_SIMM19_SHIFT, SIMM19_NUM_BITS + TRANSFER_SIMM19_SHIFT);
    uint64_t address = cpu->PC + offset;

    write_register(cpu, rt, read_bytes_memory_reverse(cpu, address, NUM_BYTES + NUM_BYTES * sf), sf);
    return 1;
}

int transfer_instruction(CPU* cpu, uint32_t instr){
    if(parse_ins(instr, INSTR_TOP_BITS)) {
        // single data transfer
        return single_data(cpu, instr);
    } else {
        // load literal
        return load_literal(cpu, instr);
    }
}
