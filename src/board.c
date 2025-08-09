#include "board.h"

#include <ctype.h>
#include <stdio.h>

char *get_disk_str(const Disk disk) {
    switch (disk) {
        case BLACK:
            return "Black (X)";
            break;
        case WHITE:
            return "White (O)";
            break;
        default:
            break;
    }

    return NULL;
}

int get_index_from_pos_str(const char *str) {
    int x = tolower(str[0]) - 'a' + 1;
    int y = str[1] - '0';
    return y * BOARD_LENGTH + x;
}

char *get_pos_str(const int index) {
    static char buffer[3] = {"\0\0\0"};

    buffer[0] = 'a' + (index % BOARD_LENGTH - 1);
    buffer[1] = '0' + (index / BOARD_LENGTH);

    return buffer;
}

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

// Get a flippable count of the move for a one direction
static int get_flip_count_in_line(const Board *board, const Disk disk,
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

bool is_valid_move(const Board *board, const Disk disk, const int index) {
    return ((get_flip_count_in_line(board, disk, index, TOP) > 0) ||
            (get_flip_count_in_line(board, disk, index, TOP_RIGHT) > 0) ||
            (get_flip_count_in_line(board, disk, index, RIGHT) > 0) ||
            (get_flip_count_in_line(board, disk, index, BOTTOM_RIGHT) > 0) ||
            (get_flip_count_in_line(board, disk, index, BOTTOM) > 0) ||
            (get_flip_count_in_line(board, disk, index, BOTTOM_LEFT) > 0) ||
            (get_flip_count_in_line(board, disk, index, LEFT) > 0) ||
            (get_flip_count_in_line(board, disk, index, TOP_LEFT) > 0));
}

bool find_valid_move(const Board *board, const Disk disk) {
    for (int y = 1; y <= GRID_NUM; ++y) {
        for (int x = 1; x <= GRID_NUM; ++x) {
            if (is_valid_move(board, disk, get_index_from_xy(x, y))) {
                return true;
            }
        }
    }

    return false;
}

// Flip disks on the board from a grid index to a one direction
static void flip_disks_in_line(Board *board, const Disk disk, const int index,
                               const Direction dir) {
    if (get_flip_count_in_line(board, disk, index, dir) == 0) {
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

// Flip disks on the board from a grid index
static void flip_disks(Board *board, const Disk disk, const int index) {
    flip_disks_in_line(board, disk, index, TOP);
    flip_disks_in_line(board, disk, index, TOP_RIGHT);
    flip_disks_in_line(board, disk, index, RIGHT);
    flip_disks_in_line(board, disk, index, BOTTOM_RIGHT);
    flip_disks_in_line(board, disk, index, BOTTOM);
    flip_disks_in_line(board, disk, index, BOTTOM_LEFT);
    flip_disks_in_line(board, disk, index, LEFT);
    flip_disks_in_line(board, disk, index, TOP_LEFT);
}

void put_disk(Board *board, const Disk disk, const int index) {
    if (board->grid[index] != NONE) {
        return;
    }

    flip_disks(board, disk, index);

    board->grid[index] = disk;

    ++board->count[disk];
    --board->count[NONE];
}

int count_disks(const Board *board, const Disk disk) {
    return board->count[disk];
}

void init_board(Board *board) {
    // Clear a playable area
    for (int i = BOARD_LENGTH; i < (BOARD_LENGTH * BOARD_LENGTH) - BOARD_LENGTH;
         ++i) {
        board->grid[i] = NONE;
    }

    // Put walls
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        board->grid[get_index_from_xy(i, 0)] = WALL;
        board->grid[get_index_from_xy(i, (GRID_NUM + 1))] = WALL;
        board->grid[get_index_from_xy(0, i)] = WALL;
        board->grid[get_index_from_xy((GRID_NUM + 1), i)] = WALL;
    }

    // Put center disks
    board->grid[get_index_from_xy(4, 4)] = WHITE;
    board->grid[get_index_from_xy(4, 5)] = BLACK;
    board->grid[get_index_from_xy(5, 4)] = BLACK;
    board->grid[get_index_from_xy(5, 5)] = WHITE;

    // Reset the counts
    board->count[NONE] = 60;
    board->count[BLACK] = 2;
    board->count[WHITE] = 2;
}

void print_board(const Board *board, const Disk turn) {
    // Col. index
    puts("    a b c d e f g h");
    puts("  +-----------------+");

    for (int y = 1; y <= GRID_NUM; ++y) {
        // Row index
        printf("%d |", y);

        for (int x = 1; x <= GRID_NUM; ++x) {
            putchar(' ');

            Disk d = board->grid[get_index_from_xy(x, y)];
            if (d == BLACK) {
                putchar('X');
            } else if (d == WHITE) {
                putchar('O');
            } else if (is_valid_move(board, turn, get_index_from_xy(x, y))) {
                putchar('*');
            } else {
                putchar('-');
            }
        }

        puts(" |");
    }

    puts("  +-----------------+");
}
