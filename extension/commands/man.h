#pragma once
#include "../shell.h"
#include "../command.h"


void man(Shell* shell, Command* cmd, bool redirect, char* redirect_path, bool append);
