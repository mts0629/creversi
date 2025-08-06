#include "com.h"

#include <stdlib.h>

void get_com_move(Board *board, int *x, int *y, const Disk current) {
    while (true) {
        // [TODD] implement a game AI
        *x = rand() % GRID_NUM + 1;
        *y = rand() % GRID_NUM + 1;

        if (is_valid_move(board, current, xy_to_index(*x, *y))) {
            break;
        }
    }
}
