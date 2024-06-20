#include "mv.h"
#include "cp.h"


/*
Copies the files in srcs to dest.
srcs is an array of paths.
*/
void mv(Shell* shell, char** srcs, int src_count, char* dest) {
    cp_mv(shell, srcs, src_count, dest, true);
}
