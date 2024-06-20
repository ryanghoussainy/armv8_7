#include "pwd.h"
#include "echo.h"

void pwd(Shell* shell, bool redirect, char* redirect_path, bool append) {
    if (redirect) {
        echo(shell, shell->path, true, redirect_path, append);
    } else {
        fprintf(shell->out, "%s\n", shell->path);
    }
}