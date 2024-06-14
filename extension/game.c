#include "game.h"

bool is_over(Game game) {
    return game->current_round >= game->total_rounds;
}

Result winner(Game game) {
    if (!is_over(game)) {
        // error
        printf("Game is not over yet");
        exit(1);
    }
}
