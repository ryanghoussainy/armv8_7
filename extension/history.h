#include <stdio.h>
#include <stdint.h>

#include "constants.h"

typedef enum Multiplier {
    NORMAL,
    SPARE,
    STRIKE
} Multiplier;

struct History {
    int pin_count[TOTAL_ROUNDS][2];
    Multiplier multipliers[TOTAL_ROUNDS];
};
typedef struct History *History;

History init_history();
void history_append(History h, int round, int pin_count_1, int pin_count_2);
