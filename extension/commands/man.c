#include "man.h"
#include "echo.h"


/*
Prints the manual of a command.
*/
void man(Shell* shell, char* cmd, bool redirect, char* redirect_path, bool append) {
    char* manual = get_manual(parse_to_operation(cmd));
    if (redirect) {
        echo(shell, manual, true, redirect_path, append);
    } else {
        fprintf(shell->out, "%s\n", manual);
    }
}
