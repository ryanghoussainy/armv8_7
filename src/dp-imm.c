#include "dp-imm.h"
#include <limits.h>

DPImmComponents get_dp_imm_components(uint32_t instr)
{
    DPImmComponents components = {
        parse_ins(instr, 31, 31), // sf
        parse_ins(instr, 29, 30), // opc
        parse_ins(instr, 23, 25), // opi
        parse_ins(instr, 0, 4),   // rd
        parse_ins(instr, 22, 22), // sh
        parse_ins(instr, 10, 21), // imm12
        parse_ins(instr, 5, 9),   // rn
        parse_ins(instr, 21, 22), // hw
        parse_ins(instr, 5, 20)   // imm16
    };

    return components;
}

int dp_imm_instruction(CPU* cpu, uint32_t instr)
{
    DPImmComponents components = get_dp_imm_components(instr);
    DPImmComponents* components_ptr = &components;

    if (components.opi == 2) {
        return do_arithmetic(cpu, components_ptr);
    }else if (components.opi == 5) {
        return do_wide_move(cpu, components_ptr);
    }
    
    printf("Invalid DP Immediate instruction\n");
    return 0;
}

int do_arithmetic(CPU* cpu, DPImmComponents* components)
{
    uint64_t op2 = components->sh ? components->imm12 << 12 : components->imm12;
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rd = arithmetic_operation(cpu, components->sf, components->opc, Rn, op2);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

int do_wide_move(CPU* cpu, DPImmComponents* components)
{
    uint64_t operand_value = components->imm16 << (components->hw * 16);

    if (components->opc == 0) {
        write_register(cpu, components->rd, ~operand_value, components->sf);
    }else if (components->opc == 2) {
        write_register(cpu, components->rd, operand_value, components->sf);
    }else if (components->opc == 3) {
        uint64_t register_value = read_register(cpu, components->rd, components->sf);
        uint64_t keep_mask = ~build_mask(components->hw*16, components->hw*16 + 15);
        uint64_t new_rd_value = (register_value & keep_mask) | operand_value;

        write_register(cpu, components->rd, new_rd_value, components->sf);
    }else {
        printf("Invalid wide move instruction\n");
        return 0;
    }

    return 1;
}