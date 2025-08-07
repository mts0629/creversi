#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

// Disk on the board
typedef enum { NONE = 0, BLACK, WHITE, WALL } Disk;

// Size of grid
#define GRID_NUM 8
// Board size with sentinels
#define BOARD_LENGTH (GRID_NUM + 2)

typedef struct {
    Disk grid[BOARD_LENGTH * BOARD_LENGTH];
    int count[3];
} Board;

// Convert (x, y) to a grid index
static inline int xy_to_index(const int x, const int y) {
    return y * BOARD_LENGTH + x;
}

// Get an opposite color of the disk
static inline Disk get_opposite(const Disk disk) {
    return (disk == BLACK) ? WHITE : BLACK;
}

bool is_valid_move(const Board *board, const Disk disk, const int index);

bool find_valid_move(const Board *board, const Disk disk);

void put_disk(Board *board, const Disk disk, const int index);

int count_disks(const Board *board, const Disk disk);

void init_board(Board *board);

void print_board(const Board *board, const Disk turn);

void get_xy(int *x, int *y, const char *str);

char *get_pos_str(const int x, const int y);

#endif
