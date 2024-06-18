#include "pwd.h"

void pwd(Shell* shell) {
    fprintf(shell->out, "%s\n", shell->path);
}