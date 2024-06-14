#include "player.h"


Player init_player(char name[20]) {
    Player p = malloc(sizeof(struct Player));
    strcpy(p->name, name);
    p->history = init_history();
    return p;
}


void bowl(Player p) {
    // TODO
    // calls function from detect-pins and goes into loop to detect pins

    // do that again unless it was a strike (i.e. second roll)

    // updates player history
    
}
