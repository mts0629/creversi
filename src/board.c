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

// Get a disk at (x, y)
static inline Disk *disk_at(Disk *board, const int x, const int y) {
    return &board[y * BOARD_LENGTH + x];
}

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
static int get_reversible_count_in_line(Disk *board, const Disk disk,
                                        const int index, const Direction dir) {
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

    if (board[cur_index] != disk) {
        return 0;
    }

    return count;
}

// Convert (x, y) to a grid index
static inline int xy_to_grid_index(const int x, const int y) {
    return y * BOARD_LENGTH + x;
}

// Check whether the move (x, y) is valid or not
bool is_valid_move(const Disk disk, const int x, const int y) {
    int index = xy_to_grid_index(x, y);
    return (
        (get_reversible_count_in_line(board, disk, index, TOP) > 0) ||
        (get_reversible_count_in_line(board, disk, index, TOP_RIGHT) > 0) ||
        (get_reversible_count_in_line(board, disk, index, RIGHT) > 0) ||
        (get_reversible_count_in_line(board, disk, index, BOTTOM_RIGHT) > 0) ||
        (get_reversible_count_in_line(board, disk, index, BOTTOM) > 0) ||
        (get_reversible_count_in_line(board, disk, index, BOTTOM_LEFT) > 0) ||
        (get_reversible_count_in_line(board, disk, index, LEFT) > 0) ||
        (get_reversible_count_in_line(board, disk, index, TOP_LEFT) > 0));
}

// Find a valid move
bool find_valid_move(const Disk disk) {
    for (int i = 0; i < GRID_NUM; ++i) {
        for (int j = 0; j < GRID_NUM; ++j) {
            if (is_valid_move(disk, i, j)) {
                return true;
            }
        }
    }

    return false;
}

// Reverse disks on the board from a grid index to a one direction
static void reverse_disks_in_line(Disk *board, const Disk disk, const int index,
                                  const Direction dir) {
    if (get_reversible_count_in_line(board, disk, index, dir) == 0) {
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
static void reverse_disks(Disk *board, const Disk disk, const int index) {
    reverse_disks_in_line(board, disk, index, TOP);
    reverse_disks_in_line(board, disk, index, TOP_RIGHT);
    reverse_disks_in_line(board, disk, index, RIGHT);
    reverse_disks_in_line(board, disk, index, BOTTOM_RIGHT);
    reverse_disks_in_line(board, disk, index, BOTTOM);
    reverse_disks_in_line(board, disk, index, BOTTOM_LEFT);
    reverse_disks_in_line(board, disk, index, LEFT);
    reverse_disks_in_line(board, disk, index, TOP_LEFT);
}

// Put a disk on the board
void put_disk(const Disk disk, const int x, const int y) {
    int index = xy_to_grid_index(x, y);

    if (board[index] != NONE) {
        return;
    }

    reverse_disks(board, disk, index);

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
    for (int i = 1; i <= GRID_NUM; ++i) {
        for (int j = 1; j <= GRID_NUM; ++j) {
            *disk_at(board, i, j) = NONE;
        }
    }

    // Put edges
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        *disk_at(board, i, 0) = EDGE;
        *disk_at(board, i, (GRID_NUM + 1)) = EDGE;
        *disk_at(board, 0, i) = EDGE;
        *disk_at(board, (GRID_NUM + 1), i) = EDGE;
    }

    // Put center disks
    *disk_at(board, 4, 4) = WHITE;
    *disk_at(board, 4, 5) = BLACK;
    *disk_at(board, 5, 4) = BLACK;
    *disk_at(board, 5, 5) = WHITE;
}

// Print the current board
void print_board(const Disk next) {
    // Col. index
    puts("    a b c d e f g h");
    puts("  +-----------------+");

    for (int i = 1; i <= GRID_NUM; ++i) {
        // Row index
        printf("%d |", i);

        for (int j = 1; j <= GRID_NUM; ++j) {
            putchar(' ');

            Disk *pd = disk_at(board, j, i);
            if (*pd == BLACK) {
                putchar('X');
            } else if (*pd == WHITE) {
                putchar('O');
            } else if (is_valid_move(next, j, i)) {
                putchar('*');
            } else {
                putchar('-');
            }
        }

        puts(" |");
    }

    puts("  +-----------------+");
}
