#include "com.h"

#include <stdlib.h>

int get_com_move(Board *board, const Disk current) {
    int index;

    while (true) {
        // [TODD] implement a game AI
        int x = rand() % GRID_NUM + 1;
        int y = rand() % GRID_NUM + 1;

        index = get_index_from_xy(x, y);

        if (is_valid_move(board, current, index)) {
            break;
        }
    }

    return index;
}
