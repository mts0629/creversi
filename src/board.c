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
#define GRID_NUM 8
#define BOARD_LENGTH (GRID_NUM + 2)  // Board size with sentinels
Disk board[BOARD_LENGTH * BOARD_LENGTH];

// Get a disk at (x, y)
static inline Disk *disk_at(Disk *board, const int x, const int y) {
    return &board[y * BOARD_LENGTH + x];
}

// Proceed a position (x, y) by the direction
static void proceed_xy(int *x, int *y, const Direction dir) {
    switch (dir) {
        case TOP:
            (*y)--;
            break;
        case TOP_RIGHT:
            (*x)++;
            (*y)--;
            break;
        case RIGHT:
            (*x)++;
            break;
        case BOTTOM_RIGHT:
            (*x)++;
            (*y)++;
            break;
        case BOTTOM:
            (*y)++;
            break;
        case BOTTOM_LEFT:
            (*x)--;
            (*y)++;
            break;
        case LEFT:
            (*x)--;
            break;
        case TOP_LEFT:
            (*x)--;
            (*y)--;
            break;
        default:
            break;
    }
}

// Get a reversible count of the move (x, y) for a one direction
static int get_reversible_count_in_line(Disk *board, const Disk disk,
                                        const int x, const int y,
                                        const Direction dir) {
    Disk opposite = get_opposite(disk);

    Disk *pd = disk_at(board, x, y);
    if (*pd != NONE) {
        return 0;
    }

    int px = x, py = y;
    proceed_xy(&px, &py, dir);

    int count = 0;
    while (*(pd = disk_at(board, px, py)) == opposite) {
        ++count;
        proceed_xy(&px, &py, dir);
    }

    if (*pd != disk) {
        return 0;
    }

    return count;
}

// Check whether the move (x, y) is valid or not
bool is_valid_move(const Disk disk, const int x, const int y) {
    return (
        (get_reversible_count_in_line(board, disk, x, y, TOP) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, TOP_RIGHT) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, RIGHT) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, BOTTOM_RIGHT) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, BOTTOM) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, BOTTOM_LEFT) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, LEFT) > 0) ||
        (get_reversible_count_in_line(board, disk, x, y, TOP_LEFT) > 0));
}

// Reverse disks on the board from (x, y) for a one direction
static void reverse_disks_in_line(Disk *board, const Disk disk, const int x,
                                  const int y, const Direction dir) {
    Disk opposite = get_opposite(disk);

    int px = x, py = y;
    if (get_reversible_count_in_line(board, disk, x, y, dir) == 0) {
        return;
    }

    proceed_xy(&px, &py, dir);

    Disk *pd;
    while (*(pd = disk_at(board, px, py)) == opposite) {
        *pd = disk;
        proceed_xy(&px, &py, dir);
    }
}

// Reverse disks on the board from (x, y)
static void reverse_disks(Disk *board, const Disk disk, const int x,
                          const int y) {
    reverse_disks_in_line(board, disk, x, y, TOP);
    reverse_disks_in_line(board, disk, x, y, TOP_RIGHT);
    reverse_disks_in_line(board, disk, x, y, RIGHT);
    reverse_disks_in_line(board, disk, x, y, BOTTOM_RIGHT);
    reverse_disks_in_line(board, disk, x, y, BOTTOM);
    reverse_disks_in_line(board, disk, x, y, BOTTOM_LEFT);
    reverse_disks_in_line(board, disk, x, y, LEFT);
    reverse_disks_in_line(board, disk, x, y, TOP_LEFT);
}

// Put a disk on the board
void put_disk(const Disk disk, const int x, const int y) {
    Disk *pd = disk_at(board, x, y);
    if (*pd != NONE) {
        return;
    }

    reverse_disks(board, disk, x, y);

    *pd = disk;
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
