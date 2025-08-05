#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

    srand(time(NULL));
}

// Print a prompt
static void print_prompt(const Disk turn) {
    if (turn == BLACK) {
        printf("Black (X) turn > \033[0K");
    } else {
        printf("White (O) turn > \033[0K");
    }
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
            get_xy(&tx, &ty, buffer);

            // Validate (x, y)
            if ((tx >= 1) && (tx <= GRID_NUM) && (ty >= 1) &&
                (ty <= GRID_NUM)) {
                *x = tx;
                *y = ty;
                break;
            }
        }

        printf(
            "\r\033[0K"
            "Invalid input: specify a coordinate as \"a1\"-\"h8\" "
            "(\"A1\"-\"H8\")"
            "\033[1A\r");
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
        print_board(current);

        if (!find_valid_move(current)) {
            Disk opposite = get_opposite(current);

            if (!find_valid_move(opposite)) {
                break;
            }

            print_prompt(current);
            printf("pass\n");

            current = opposite;

            // Rewind the cursor
            printf("\r\033[12A");
            continue;
        }

        int x, y;
        if (current == player) {
            while (true) {
                char *buffer = get_input(&x, &y, current);

                printf("\r\033[0K");

                // Quit a game
                if (str_eq(buffer, "q")) {
                    printf("Quit the game\n");
                    return;
                }

                if (is_valid_move(current, xy_to_index(x, y))) {
                    break;
                }

                printf(
                    "Invalid move: cannot put at \"%s\""
                    "\033[1A\r",
                    buffer);
            }
        } else {
            get_com_move(&x, &y, current);

            printf("\n");
            print_prompt(current);
            printf("%s", get_pos_str(x, y));
        }

        put_disk(current, xy_to_index(x, y));

        current = get_opposite(current);

        // Rewind the cursor
        printf("\r\033[12A");
    }

    judge_game();
}
