#include "shell.h"
#include "ls.h"

int main(void) {
    Shell shell = { NULL, NULL, "./", stdout };
    ls(&shell);
    return 0;
}