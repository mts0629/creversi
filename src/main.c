#include <ctype.h>
#include <stdio.h>

#include "board.h"

// Print a prompt
void print_prompt(const Disk next) {
    if (next == BLACK) {
        printf("Black (X) turn > ");
    } else {
        printf("White (O) turn > ");
    }
}

// Parse the player input
bool parse_input(int *x, int *y, const char *buffer) {
    if (isalpha(buffer[0]) && isdigit(buffer[1])) {
        // Convert to (x, y) coordinate
        int tx = tolower(buffer[0]) - 'a' + 1;
        int ty = buffer[1] - '0';

        // Validate (x, y)
        if ((tx >= 1) || (tx <= GRID_NUM) || (ty >= 1) || (ty <= GRID_NUM)) {
            *x = tx;
            *y = ty;
            return true;
        }
    }

    // 'q' is quit
    if (tolower(buffer[0]) == 'q') {
        return true;
    }

    return false;
}

// Judge a game
void judge(void) {
    int num_black = count_disks(BLACK);
    int num_white = count_disks(WHITE);

    printf("B:W=%d:%d\n", num_black, num_white);
    if (num_black > num_white) {
        printf("Black wins!\n");
    } else if (num_white > num_black) {
        printf("White wins!\n");
    } else {
        printf("Draw\n");
    }
}

int main(void) {
    init_board();

    Disk next = BLACK;

    int turn = 0;
    bool playable[2] = {true, true};

    while (playable[0] || playable[1]) {
        // Check existing valid moves
        if (!find_valid_move(next)) {
            playable[turn % 2] = false;
            next = get_opposite(next);

            printf("Skip a turn\n");
            ++turn;
            continue;
        }

        playable[turn % 2] = true;

        print_board(next);

        print_prompt(next);

        // Get a player input
        char buf[4];
        fgets(buf, sizeof(buf), stdin);

        int x, y;
        if (!parse_input(&x, &y, buf)) {
            printf("Invaid input: specify a0 (/A1) - h1 (/H1)\n");
            continue;
        }

        // Quit a game
        if (buf[0] == 'q') {
            break;
        }

        if (!is_valid_move(next, x, y)) {
            printf("Invalid move\n");
            continue;
        }

        put_disk(next, x, y);

        next = get_opposite(next);

        ++turn;
    }

    if (!playable[0] && !playable[1]) {
        print_board(next);

        judge();
    }

    return 0;
}
