#include "dp-imm.h"
#include "masks.h"
#include <limits.h>

DPImmComponents get_dp_imm_components(uint32_t instruction)
{
    uint64_t sf_mask = build_mask(31, 31);
    uint64_t ofc_mask = build_mask(29, 30);
    uint64_t opi_mask = build_mask(23, 25);
    uint64_t rd_mask = build_mask(0, 4);
    uint64_t sh_mask = build_mask(22, 22);
    uint64_t imm12_mask = build_mask(10, 21);
    uint64_t rn_mask = build_mask(5, 9);
    uint64_t hw_mask = build_mask(21, 22);
    uint64_t imm16_mask = build_mask(5, 20);

    uint64_t sf = (instruction & sf_mask) >> 31;
    uint64_t ofc = (instruction & ofc_mask) >> 29;
    uint64_t opi = (instruction & opi_mask) >> 23;
    uint64_t rd = (instruction & rd_mask);
    uint64_t sh = (instruction & sh_mask) >> 22;
    uint64_t imm12 = (instruction & imm12_mask) >> 10;
    uint64_t rn = (instruction & rn_mask) >> 5;
    uint64_t hw = (instruction & hw_mask) >> 21;
    uint64_t imm16 = (instruction & imm16_mask) >> 5;

    DPImmComponents components = {
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

int dp_imm_instruction(CPU* cpu, uint32_t instruction)
{
    DPImmComponents components = get_dp_imm_components(instruction);
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