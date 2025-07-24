#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "board.h"

static Disk current;

// Initialize the game
void init_game(void) {
    init_board();

    current = BLACK;
}

// Print a prompt
static void print_prompt(const Disk turn) {
    if (turn == BLACK) {
        printf("Black (X) turn > ");
    } else {
        printf("White (O) turn > ");
    }
}

// Parse a string to (x, y) coordinate
static void parse_to_xy(int *x, int *y, const char *buffer) {
    *x = tolower(buffer[0]) - 'a' + 1;
    *y = buffer[1] - '0';
}

// Get the player input
static char *get_input(int *x, int *y, const Disk turn) {
    static char buffer[3 + 1];

    while (true) {
        print_prompt(turn);

        fgets(buffer, sizeof(buffer), stdin);

        // Flush redundant input
        size_t len = strlen(buffer);
        if (buffer[len - 1] != '\n') {
            while (getchar() != '\n')
                ;
        }

        // Remove '\n'
        for (size_t i = 0; i < sizeof(buffer); ++i) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }

        // Validate a quit command ('q')
        if (strcmp(buffer, "q") == 0) {
            break;
        }

        if (strlen(buffer) == 2) {
            int tx, ty;
            parse_to_xy(&tx, &ty, buffer);

            // Validate (x, y)
            if ((tx >= 1) && (tx <= GRID_NUM) && (ty >= 1) &&
                (ty <= GRID_NUM)) {
                *x = tx;
                *y = ty;
                break;
            }
        }

        printf(
            "Invaid input: specify a coordinate as \"a1\"-\"h8\" "
            "(\"A1\"-\"H8\")\n");
    }

    return buffer;
}

// Play a game
bool play_game(void) {
    while (true) {
        if (!find_valid_move(current)) {
            current = get_opposite(current);

            if (!find_valid_move(current)) {
                break;
            }

            printf("Pass\n");
        }

        print_board(current);

        int x, y;
        char *buffer = get_input(&x, &y, current);

        // Quit a game
        if (strcmp(buffer, "q") == 0) {
            printf("Quit the game\n");
            return false;
        }

        if (!is_valid_move(current, x, y)) {
            printf("Invalid move: cannot put at \"%s\"\n", buffer);
            continue;
        }

        put_disk(current, x, y);

        current = get_opposite(current);
    }

    return true;
}

// Judge a game
void judge_game(void) {
    print_board(current);

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
