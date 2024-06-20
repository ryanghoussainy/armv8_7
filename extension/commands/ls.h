#pragma once

#include <stdio.h> 
#include "../shell.h"
#include "../files.h"

void ls(Shell* shell, char* path, bool redirect, char* redirect_path, bool append);