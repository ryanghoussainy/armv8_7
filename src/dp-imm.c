#include "dp-imm.h"

static DPImmComponents get_dp_imm_components(uint32_t instr)
{
    DPImmComponents components = {
        parse_ins(instr, SF_BITS), // sf
        parse_ins(instr, OPC_BITS), // opc
        parse_ins(instr, OPI_BITS), // opi
        parse_ins(instr, RD_BITS),   // rd
        parse_ins(instr, SH_BITS), // sh
        parse_ins(instr, IMM12_BITS), // imm12
        parse_ins(instr, RN_BITS),   // rn
        parse_ins(instr, HW_BITS), // hw
        parse_ins(instr, IMM16_BITS)   // imm16
    };

    return components;
}

static int do_arithmetic(CPU* cpu, DPImmComponents* components)
{
    uint64_t op2 = components->sh ? components->imm12 << IMM12_SHIFT : components->imm12;
    uint64_t Rn = read_register(cpu, components->rn, components->sf);
    uint64_t Rd = arithmetic_operation(cpu, components->sf, components->opc, Rn, op2);
    write_register(cpu, components->rd, Rd, components->sf);
    return 1;
}

static int do_wide_move(CPU* cpu, DPImmComponents* components)
{
    uint64_t operand_value = components->imm16 << (components->hw * IMM16_VAL);

    switch (components->opc) {
        case 0: // Move wide with NOT
            write_register(cpu, components->rd, ~operand_value, components->sf);
            break;
        case 2: // Move wide with zero
            write_register(cpu, components->rd, operand_value, components->sf);
            break;
        case 3:; // Move wide with keep
            uint64_t register_value = read_register(cpu, components->rd, components->sf);
            uint64_t keep_mask = ~build_mask(components->hw*IMM16_VAL, components->hw*IMM16_VAL + IMM16_VAL - 1);
            uint64_t new_rd_value = (register_value & keep_mask) | operand_value;

            write_register(cpu, components->rd, new_rd_value, components->sf);
            break;
        default:
            printf("Invalid wide move instruction\n");
            return 0;
    }
    return 1;
}

int dp_imm_instruction(CPU* cpu, uint32_t instr)
{
    DPImmComponents components = get_dp_imm_components(instr);
    DPImmComponents* components_ptr = &components;

    switch (components.opi) {
        case 2: // Arithmetic operation
            return do_arithmetic(cpu, components_ptr);
        case 5: // Wide Move
            return do_wide_move(cpu, components_ptr);
        default:
            printf("Invalid DP Immediate instruction\n");
            return 0;
    }
}
