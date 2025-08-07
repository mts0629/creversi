#include "board.h"

#include <ctype.h>
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
static int get_reversible_count_in_line(const Board *board, const Disk disk,
                                        const int index, const Direction dir) {
    if (board->grid[index] != NONE) {
        return 0;
    }

    int cur_index = index + proceed_index(dir);
    Disk opposite = get_opposite(disk);
    int count = 0;
    while (board->grid[cur_index] == opposite) {
        ++count;
        cur_index += proceed_index(dir);
    }

    if (board->grid[cur_index] == disk) {
        return count;
    }

    return 0;
}

// Check whether the move (x, y) is valid or not
bool is_valid_move(const Board *board, const Disk disk, const int index) {
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
bool find_valid_move(const Board *board, const Disk disk) {
    for (int y = 1; y <= GRID_NUM; ++y) {
        for (int x = 1; x <= GRID_NUM; ++x) {
            if (is_valid_move(board, disk, xy_to_index(x, y))) {
                return true;
            }
        }
    }

    return false;
}

// Reverse disks on the board from a grid index to a one direction
static void reverse_disks_in_line(Board *board, const Disk disk,
                                  const int index, const Direction dir) {
    if (get_reversible_count_in_line(board, disk, index, dir) == 0) {
        return;
    }

    int cur_index = index + proceed_index(dir);
    Disk opposite = get_opposite(disk);
    while (board->grid[cur_index] == opposite) {
        board->grid[cur_index] = disk;

        --board->count[opposite];
        ++board->count[disk];

        cur_index += proceed_index(dir);
    }
}

// Reverse disks on the board from a grid index
static void reverse_disks(Board *board, const Disk disk, const int index) {
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
void put_disk(Board *board, const Disk disk, const int index) {
    if (board->grid[index] != NONE) {
        return;
    }

    reverse_disks(board, disk, index);

    board->grid[index] = disk;

    ++board->count[disk];
    --board->count[NONE];
}

// Count the number of disks
int count_disks(const Board *board, const Disk disk) {
    return board->count[disk];
}

// Initialize the board
void init_board(Board *board) {
    // Clear a playable area
    for (int y = 1; y <= GRID_NUM; ++y) {
        for (int x = 1; x <= GRID_NUM; ++x) {
            board->grid[xy_to_index(x, y)] = NONE;
        }
    }

    // Put edges
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        board->grid[xy_to_index(i, 0)] = WALL;
        board->grid[xy_to_index(i, (GRID_NUM + 1))] = WALL;
        board->grid[xy_to_index(0, i)] = WALL;
        board->grid[xy_to_index((GRID_NUM + 1), i)] = WALL;
    }

    // Put center disks
    board->grid[xy_to_index(4, 4)] = WHITE;
    board->grid[xy_to_index(4, 5)] = BLACK;
    board->grid[xy_to_index(5, 4)] = BLACK;
    board->grid[xy_to_index(5, 5)] = WHITE;

    board->count[NONE] = 60;
    board->count[BLACK] = 2;
    board->count[WHITE] = 2;
}

// Print the current board
void print_board(const Board *board, const Disk turn) {
    // Col. index
    puts("    a b c d e f g h");
    puts("  +-----------------+");

    for (int y = 1; y <= GRID_NUM; ++y) {
        // Row index
        printf("%d |", y);

        for (int x = 1; x <= GRID_NUM; ++x) {
            putchar(' ');

            Disk d = board->grid[xy_to_index(x, y)];
            if (d == BLACK) {
                putchar('X');
            } else if (d == WHITE) {
                putchar('O');
            } else if (is_valid_move(board, turn, xy_to_index(x, y))) {
                putchar('*');
            } else {
                putchar('-');
            }
        }

        puts(" |");
    }

    puts("  +-----------------+");
}

// Parse a string of grid position ("a1" - "h8") to (x, y) coordinate
void get_xy(int *x, int *y, const char *str) {
    *x = tolower(str[0]) - 'a' + 1;
    *y = str[1] - '0';
}

// Parse (x, y) to a string of grid position ("a1" - "h8")
char *get_pos_str(const int x, const int y) {
    static char buffer[3] = {0};

    buffer[0] = 'a' + (x - 1);
    buffer[1] = '0' + y;
    buffer[2] = '\0';

    return buffer;
}
