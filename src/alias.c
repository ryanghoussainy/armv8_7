#include "alias.h"

void handle_aliases(Instruction* instr) {
    char* rzr_str = instr->o1.reg[0] == 'x' ? "xzr" : "wzr";

    union Operand rzr;
    strcpy(rzr.reg, rzr_str);

    char* op = instr->operation;
    if (strcmp(op, "cmp") == 0 || strcmp(op, "cmn") == 0 || strcmp(op, "tst") == 0) {
        if (strcmp(op, "cmp") == 0) {
            strcpy(&op, "subs");
        } else if (strcmp(op, "cmn") == 0) {
            strcpy(&op, "adds");
        } else if (strcmp(op, "tst") == 0) {
            strcpy(&op, "ands");
        }

        instr->o3 = instr->o2;
        instr->o3_type = instr->o2_type;

        instr->o2 = instr->o1;
        instr->o2_type = instr->o1_type;

        instr->o1 = rzr;
        instr->o1_type = REGISTER;
    } else if (strstr(op, "neg") != NULL || strcmp(op, "mvn") == 0 || strcmp(op, "mov") == 0) {
        if (strcmp(op, "neg") == 0) {
            strcpy(&op, "sub");
        } else if (strcmp(op, "negs") == 0) {
            strcpy(&op, "subs");
        } else if (strcmp(op, "mvn") == 0) {
            strcpy(&op, "orn");
        } else if (strcmp(op, "mov") == 0) {
            strcpy(&op, "orr");
        }

        instr->o3 = instr->o2;
        instr->o3_type = instr->o2_type;

        instr->o2 = rzr;
        instr->o2_type = REGISTER;
    } else if (strcmp(op, "mul") == 0 || strcmp(op, "mneg") == 0) {
        if (strcmp(op, "mul") == 0) {
            strcpy(&op, "madd");
        } else if (strcmp(op, "mneg") == 0) {
            strcpy(&op, "msub");
        }

        instr->o4 = rzr;
        instr->o4_type = REGISTER;
    }
}
