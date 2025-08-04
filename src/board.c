#include "board.h"

#include <stdio.h>

// 8x8 grid with sentinels
//  +-----------------+
//  | - - - - - - - - |
//  | - - - - - - - - |
//  | - - - - - - - - |
//  | - - - - - - - - |
//  | - - - - - - - - |
//  | - - - - - - - - |
//  | - - - - - - - - |
//  | - - - - - - - - |
//  +-----------------+
#define BOARD_LENGTH (GRID_NUM + 2)  // Board size with sentinels
static Disk board[BOARD_LENGTH * BOARD_LENGTH];

// Search direction
typedef enum {
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT,
    TOP_LEFT
} Direction;

// Proceed an index by the direction
static int proceed_index(const Direction dir) {
    switch (dir) {
        case TOP:
            return -BOARD_LENGTH;
            break;
        case TOP_RIGHT:
            return -(BOARD_LENGTH - 1);
            break;
        case RIGHT:
            return 1;
            break;
        case BOTTOM_RIGHT:
            return (BOARD_LENGTH + 1);
            break;
        case BOTTOM:
            return BOARD_LENGTH;
            break;
        case BOTTOM_LEFT:
            return (BOARD_LENGTH - 1);
            break;
        case LEFT:
            return -1;
            break;
        case TOP_LEFT:
            return -(BOARD_LENGTH + 1);
            break;
        default:
            return 0;
            break;
    }
}

// Get a reversible count of the move for a one direction
static int get_reversible_count_in_line(const Disk disk, const int index,
                                        const Direction dir) {
    if (board[index] != NONE) {
        return 0;
    }

    int cur_index = index + proceed_index(dir);
    Disk opposite = get_opposite(disk);
    int count = 0;
    while (board[cur_index] == opposite) {
        ++count;
        cur_index += proceed_index(dir);
    }

    if (board[cur_index] == disk) {
        return count;
    }

    return 0;
}

// Check whether the move (x, y) is valid or not
bool is_valid_move(const Disk disk, const int index) {
    return ((get_reversible_count_in_line(disk, index, TOP) > 0) ||
            (get_reversible_count_in_line(disk, index, TOP_RIGHT) > 0) ||
            (get_reversible_count_in_line(disk, index, RIGHT) > 0) ||
            (get_reversible_count_in_line(disk, index, BOTTOM_RIGHT) > 0) ||
            (get_reversible_count_in_line(disk, index, BOTTOM) > 0) ||
            (get_reversible_count_in_line(disk, index, BOTTOM_LEFT) > 0) ||
            (get_reversible_count_in_line(disk, index, LEFT) > 0) ||
            (get_reversible_count_in_line(disk, index, TOP_LEFT) > 0));
}

// Find a valid move
bool find_valid_move(const Disk disk) {
    for (int y = 1; y <= GRID_NUM; ++y) {
        for (int x = 1; x <= GRID_NUM; ++x) {
            if (is_valid_move(disk, xy_to_index(x, y))) {
                return true;
            }
        }
    }

    return false;
}

// Reverse disks on the board from a grid index to a one direction
static void reverse_disks_in_line(const Disk disk, const int index,
                                  const Direction dir) {
    if (get_reversible_count_in_line(disk, index, dir) == 0) {
        return;
    }

    int cur_index = index + proceed_index(dir);
    Disk opposite = get_opposite(disk);
    while (board[cur_index] == opposite) {
        board[cur_index] = disk;
        cur_index += proceed_index(dir);
    }
}

// Reverse disks on the board from a grid index
static void reverse_disks(const Disk disk, const int index) {
    reverse_disks_in_line(disk, index, TOP);
    reverse_disks_in_line(disk, index, TOP_RIGHT);
    reverse_disks_in_line(disk, index, RIGHT);
    reverse_disks_in_line(disk, index, BOTTOM_RIGHT);
    reverse_disks_in_line(disk, index, BOTTOM);
    reverse_disks_in_line(disk, index, BOTTOM_LEFT);
    reverse_disks_in_line(disk, index, LEFT);
    reverse_disks_in_line(disk, index, TOP_LEFT);
}

// Put a disk on the board
void put_disk(const Disk disk, const int index) {
    if (board[index] != NONE) {
        return;
    }

    reverse_disks(disk, index);

    board[index] = disk;
}

// Count the number of disks
int count_disks(const Disk disk) {
    int count = 0;

    for (int i = (BOARD_LENGTH + 1);
         i < (BOARD_LENGTH * BOARD_LENGTH - BOARD_LENGTH); ++i) {
        if (board[i] == disk) {
            ++count;
        }
    }

    return count;
}

// Initialize the board
void init_board(void) {
    // Clear a playable area
    for (int y = 1; y <= GRID_NUM; ++y) {
        for (int x = 1; x <= GRID_NUM; ++x) {
            board[xy_to_index(x, y)] = NONE;
        }
    }

    // Put edges
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        board[xy_to_index(i, 0)] = EDGE;
        board[xy_to_index(i, (GRID_NUM + 1))] = EDGE;
        board[xy_to_index(0, i)] = EDGE;
        board[xy_to_index((GRID_NUM + 1), i)] = EDGE;
    }

    // Put center disks
    board[xy_to_index(4, 4)] = WHITE;
    board[xy_to_index(4, 5)] = BLACK;
    board[xy_to_index(5, 4)] = BLACK;
    board[xy_to_index(5, 5)] = WHITE;
}

// Print the current board
void print_board(const Disk turn) {
    // Col. index
    puts("    a b c d e f g h");
    puts("  +-----------------+");

    for (int y = 1; y <= GRID_NUM; ++y) {
        // Row index
        printf("%d |", y);

        for (int x = 1; x <= GRID_NUM; ++x) {
            putchar(' ');

            Disk d = board[xy_to_index(x, y)];
            if (d == BLACK) {
                putchar('X');
            } else if (d == WHITE) {
                putchar('O');
            } else if (is_valid_move(turn, xy_to_index(x, y))) {
                putchar('*');
            } else {
                putchar('-');
            }
        }

        puts(" |");
    }

    puts("  +-----------------+");
}
