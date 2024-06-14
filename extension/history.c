#include "history.h"

History init_history() {
    History h = malloc(sizeof(struct History));
    for (int i = 0; i < TOTAL_ROUNDS; i++) {
        h->multipliers[i] = NORMAL;
        h->pin_count[i][0] = -1;
        h->pin_count[i][1] = -1;
    }
    return h;
}


static void set_multiplier(History h, int round) {
    if (h->pin_count[round][0] == PIN_COUNT) {
        h->multipliers[round] = STRIKE;
    } else if (h->pin_count[round][0] + h->pin_count[round][1] == PIN_COUNT) {
        h->multipliers[round] = SPARE;
    } else {
        h->multipliers[round] = NORMAL;
    }
}

void history_append(History h, int round, int pin_count_1, int pin_count_2) {
    h->pin_count[round][0] = pin_count_1;
    h->pin_count[round][1] = pin_count_2;
    set_multiplier(h, round);
}

int get_score_in_round(History h, int round) {
    // TODO
}

int get_score(History h) {
    // TODO
}

