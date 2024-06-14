#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "player.h"


typedef enum Result {
    P1,
    P2,
    DRAW
} Result;
 
struct Game {
    Player p1;
    Player p2;
    int total_rounds;
    int current_round;
};

typedef struct Game *Game;
