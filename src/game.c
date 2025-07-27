#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "com.h"

static Disk current;
static Disk player = BLACK;

// Check equality of two strings
static inline bool str_eq(const char *s1, const char *s2) {
    return (strcmp(s1, s2) == 0);
}

// Parse commandline arguments
void parse_args(const int argc, const char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (str_eq("-b", argv[i])) {
            player = BLACK;
        } else if (str_eq("-w", argv[i])) {
            player = WHITE;
        }
    }
}

// Initialize the game
static void init_game(void) {
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

// Parse a string of grid position ("a1" - "h8") to (x, y) coordinate
static inline void parse_grid_str_to_xy(int *x, int *y, const char *str) {
    *x = tolower(str[0]) - 'a' + 1;
    *y = str[1] - '0';
}

// Parse (x, y) to a string of grid position ("a1" - "h8")
static inline char *parse_xy_to_grid_str(const int x, const int y) {
    static char buffer[3] = {0};

    buffer[0] = 'a' + (x - 1);
    buffer[1] = '0' + y;
    buffer[2] = '\0';

    return buffer;
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
        if (str_eq(buffer, "q")) {
            break;
        }

        if (strlen(buffer) == 2) {
            int tx, ty;
            parse_grid_str_to_xy(&tx, &ty, buffer);

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

// Judge a game
static void judge_game(void) {
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

// Play a game
void play_game(void) {
    init_game();

    while (true) {
        if (!find_valid_move(current)) {
            current = get_opposite(current);

            if (!find_valid_move(current)) {
                break;
            }

            print_prompt(current);
            printf("pass\n");
        }

        print_board(current);

        int x, y;
        if (current == player) {
            char *buffer = get_input(&x, &y, current);

            // Quit a game
            if (str_eq(buffer, "q")) {
                printf("Quit the game\n");
                return;
            }

            if (!is_valid_move(current, x, y)) {
                printf("Invalid move: cannot put at \"%s\"\n", buffer);
                continue;
            }
        } else {
            get_com_move(&x, &y, current);

            print_prompt(current);
            printf("%s\n", parse_xy_to_grid_str(x, y));
        }

        put_disk(current, x, y);

        current = get_opposite(current);
    }

    judge_game();
}
