#include <stdio.h>

#include "board.h"

int main(void) {
    init_board();

    Disk next = BLACK;
    print_board(next);

    int x = 3, y = 4;
    if (is_valid_move(next, x, y)) {
        put_disk(next, x, y);
    }

    next = get_opposite(next);

    print_board(next);

    return 0;
}
