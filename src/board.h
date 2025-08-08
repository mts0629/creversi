#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

// Disk on the board
typedef enum { NONE = 0, BLACK, WHITE, WALL } Disk;

// Size of grid
#define GRID_NUM 8
// Board size with sentinels
#define BOARD_LENGTH (GRID_NUM + 2)

// Board data
typedef struct {
    Disk grid[BOARD_LENGTH * BOARD_LENGTH];
    int count[3];
} Board;

// Convert (x, y) to a grid index
static inline int get_index_from_xy(const int x, const int y) {
    return y * BOARD_LENGTH + x;
}

// Get an opposite disk
static inline Disk get_opposite(const Disk disk) {
    return BLACK + WHITE - disk;
}

// Get a string expr of the disk
char *get_disk_str(const Disk disk);

// Parse a string of grid position ("a1" - "h8") to a grid index
int get_index_from_pos_str(const char *str);

// Parse a grid index to a string of grid position ("a1" - "h8")
char *get_pos_str(const int index);

// Check whether the move is valid or not
bool is_valid_move(const Board *board, const Disk disk, const int index);

// Find a valid move
bool find_valid_move(const Board *board, const Disk disk);

// Put a disk on the board
void put_disk(Board *board, const Disk disk, const int index);

// Count the number of disks
int count_disks(const Board *board, const Disk disk);

// Initialize a board
void init_board(Board *board);

// Print the current board
void print_board(const Board *board, const Disk turn);

#endif
