#include "com.h"

#include <limits.h>
#include <stdlib.h>

// clang-format off
static const int grid_values[] = {
    0,  0,   0,  0,  0,  0,  0,   0,  0,  0,
    0, 10,  -5,  7,  3,  3,  7,  -5, 10,  0,
    0, -5, -10, -3, -3, -3, -3, -10, -5,  0,
    0,  7,  -3,  5,  2,  2,  5,  -3,  7,  0,
    0,  3,  -3,  2,  2,  2,  2,  -3,  3,  0,
    0,  3,  -3,  2,  2,  2,  2,  -3,  3,  0,
    0,  7,  -3,  5,  2,  2,  5,  -3,  7,  0,
    0, -5, -10, -3, -3, -3, -3, -10, -5,  0,
    0, 10,  -5,  7,  3,  3,  7,  -5, 10,  0,
    0,  0,   0,  0,  0,  0,  0,   0,  0,  0
};
// clang-format on

// Calculate eval. value by the current state
int calculate_value(const Board *board, const Disk disk) {
    int value = 0;

    for (int i = (BOARD_LENGTH + 1);
         i < (BOARD_LENGTH * BOARD_LENGTH) - BOARD_LENGTH; ++i) {
        if (board->grid[i] == disk) {
            value += grid_values[i];
        }
    }

    return value;
}

// Evaluation function
static int evaluate(const Board *board, const Disk com) {
    return calculate_value(board, com) +
           -calculate_value(board, get_opposite(com));
}

// Evaluate the move by depth-first search
static int evaluate_move(Board board, const Disk com, const Disk current,
                         const int depth) {
    if (depth == 0) {
        // Return evaluated value
        return evaluate(&board, com);
    }

    int valid_moves[60];
    int n = get_valid_moves(valid_moves, &board, current);

    int value = -INT_MAX;
    if (n == 0) {
        // Skip and go next turn
        return evaluate_move(board, com, get_opposite(current), (depth - 1));
    } else {
        // Search for valid moves
        for (int i = 0; i < n; ++i) {
            put_disk(&board, current, valid_moves[i]);

            int v =
                evaluate_move(board, com, get_opposite(current), (depth - 1));
            if (v > value) {
                value = v;
            }
        }
    }

    return value;
}

int get_com_next_move(const Board *board, const Disk current, const int depth) {
    int valid_moves[60];
    int n = get_valid_moves(valid_moves, board, current);

    Board copy = *board;

    int index = -1;
    int value = -INT_MAX;

    // Search the max. evaluated value and its grid index
    for (int i = 0; i < n; ++i) {
        put_disk(&copy, current, valid_moves[i]);

        int v = evaluate_move(copy, current, current, depth);

        if (v > value) {
            index = valid_moves[i];
            value = v;
        }
    }

    return index;
}
