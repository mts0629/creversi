#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

// Disk on the board
typedef enum { NONE = 0, BLACK, WHITE, EDGE } Disk;

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

// Size of grid
#define GRID_NUM 8

// Get an opposite color of the disk
static inline Disk get_opposite(const Disk disk) {
    return (disk == BLACK) ? WHITE : BLACK;
}

bool is_valid_move(const Disk disk, const int x, const int y);

bool find_valid_move(const Disk disk);

void put_disk(const Disk disk, const int x, const int y);

int count_disks(const Disk disk);

void init_board(void);

void print_board(const Disk next);

#endif
