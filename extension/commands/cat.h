#pragma once
#include "../shell.h"
#include "../files.h"

void cat(Shell* shell, char* path, bool redirect, char* redirect_path, bool append);