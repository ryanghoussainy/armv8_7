#include "man.h"
#include "echo.h"


/*
Prints the manual of a command.
*/
void man(Shell* shell, Command* cmd, bool redirect, char* redirect_path, bool append) {
    if (redirect) {
        echo(shell, cmd->manual, true, redirect_path, append);
    } else {
        fprintf(shell->out, "%s\n", cmd->manual);
    }
}
