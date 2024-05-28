#include "dp-imm.h"
#include "masks.h"
#include <limits.h>

struct DPImmComponents get_components(uint32_t instruction)
{
    uint64_t sfMask = build_mask(31, 31);
    uint64_t ofcMask = build_mask(29, 30);
    uint64_t opiMask = build_mask(23, 25);
    uint64_t rdMask = build_mask(0, 4);
    uint64_t shMask = build_mask(22, 22);
    uint64_t imm12Mask = build_mask(10, 21);
    uint64_t rnMask = build_mask(5, 9);
    uint64_t hwMask = build_mask(21, 22);
    uint64_t imm16Mask = build_mask(5, 20);

    uint64_t sf = (instruction & sfMask) >> 31;
    uint64_t ofc = (instruction & ofcMask) >> 29;
    uint64_t opi = (instruction & opiMask) >> 23;
    uint64_t rd = (instruction & rdMask);
    uint64_t sh = (instruction & shMask) >> 22;
    uint64_t imm12 = (instruction & imm12Mask) >> 10;
    uint64_t rn = (instruction & rnMask) >> 5;
    uint64_t hw = (instruction & hwMask) >> 21;
    uint64_t imm16 = (instruction & imm16Mask) >> 5;

    struct DPImmComponents components = {
        sf,
        ofc,
        opi,
        rd,
        sh,
        imm12,
        rn,
        hw,
        imm16
    };

    return components;
}

void dp_imm_instruction(struct CPU* cpu, uint32_t instruction)
{
    struct DPImmComponents components = get_components(instruction);

    if (components.opi == 2) {
        do_arithmetic(cpu, components);
    }else if (components.opi == 5) {
        do_wide_move(cpu, components);
    }
}

void do_arithmetic(struct CPU* cpu, struct DPImmComponents components)
{
    int imm = components.imm12;

    if (components.sh == 1) {
        imm <<= 12;
    }

    uint64_t result;
    uint64_t register_value = read_register(cpu, components.rn, components.sf);
    int C_flag = 0;

    if (components.opc <= 1) {
        result = register_value + imm;
        
        if (components.sf) {
            C_flag = result < register_value || result < imm;
        }else {
            C_flag = result > build_mask(0, 31);
        }

        write_register(cpu, components.rd, result, components.sf);
    }else if (components.opc <= 3) {
        result = register_value - imm;
        C_flag = imm > register_value ? 0 : 1;

        write_register(cpu, components.rd, result, components.sf);
    }

    if (components.sf == 1) {
        result = result & build_mask(0, 31);
    }

    if (components.opc != 1 && components.opc != 3) return;

    int sign_bit = 1 & (result >> (components.sf == 1 ? 63 : 31));
    set_flag(cpu, N, sign_bit);

    if (result == 0) {
        set_flag(cpu, Z, 1);
    }else {
        set_flag(cpu, Z, 0);
    }

    set_flag(cpu, C, C_flag);

    int max_int = components.sf ? INT64_MAX : INT32_MAX;
    int min_int = components.sf ? INT64_MIN : INT32_MIN;

    if (components.sf) {
        int64_t signed_result = result;
        set_flag(cpu, C, signed_result > max_int || signed_result < min_int);
    }else {
        int32_t signed_result = result;
        set_flag(cpu, Z, signed_result > max_int || signed_result < min_int);
    }
}

void do_wide_move(struct CPU* cpu, struct DPImmComponents components)
{
    uint64_t operand_value = components.imm16 << (components.hw * 16);

    if (components.opc == 0b00) {
        write_register(cpu, components.rd, ~operand_value, components.sf);
    }else if (components.opc == 0b10) {
        write_register(cpu, components.rd, operand_value, components.sf);
    }else if (components.opc == 0b11) {
        uint64_t register_value = read_register(cpu, components.rd, 1);
        uint64_t keep_mask = ~build_mask(components.hw*16, components.hw*16 + 15);
        uint64_t new_rd_value = (register_value & keep_mask) | operand_value;

        write_register(cpu, components.rd, new_rd_value, components.sf);
    }
}