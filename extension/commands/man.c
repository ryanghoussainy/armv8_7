#include "man.h"


void man(Shell* shell, Command* cmd) {
    fprintf(shell->out, "%s", cmd->manual);
}
