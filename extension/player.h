#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "history.h"

struct Player {
    char name[20];
    History history;
};
typedef struct Player *Player;

Player init_player(char name[20]);
