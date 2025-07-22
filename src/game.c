#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "board.h"

// Game state
typedef struct {
    int turn;
    Disk next;
    bool playable[2];
} Game;

Game game;

// Initialize the game
void init_game(void) {
    init_board();

    game.turn = 0;
    game.next = BLACK;
    game.playable[0] = true;
    game.playable[1] = true;
}

// Print a prompt
static void print_prompt(const Disk turn) {
    if (turn == BLACK) {
        printf("Black (X) turn > ");
    } else {
        printf("White (O) turn > ");
    }
}

static char *get_input(void) {
    static char buffer[3 + 1];

    while (true) {
        fgets(buffer, sizeof(buffer), stdin);

        // Skip redundant inputs
        size_t len = strlen(buffer);
        if (buffer[len - 1] != '\n') {
            while (getchar() != '\n') {
            }
        }

        // Remove '\n'
        for (int i = 0; i < sizeof(buffer); ++i) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }

        if (strcmp(buffer, "q") == 0) {
            break;
        }

        if (strlen(buffer) == 2) {
            if (isalpha(buffer[0])) {
                if (isdigit(buffer[1]) && (buffer[1] > '0') &&
                    (buffer[1] < '9')) {
                    break;
                }
            }
        }

        printf("Invaid input: specify a0 (/A1) - h1 (/H1)\n");
    }

    return buffer;
}

// Parse the player input
static bool parse_input(int *x, int *y, const char *buffer) {
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

void next_turn(void) {
    game.next = get_opposite(game.next);
    ++game.turn;
}

// Play a game
bool play_game(void) {
    // char buf[4];
    while (game.playable[0] || game.playable[1]) {
        // Check existing valid moves
        if (!find_valid_move(game.next)) {
            game.playable[game.turn % 2] = false;
            printf("Skip a turn\n");

            next_turn();
            continue;
        }

        game.playable[game.turn % 2] = true;

        print_board(game.next);

        print_prompt(game.next);

        // Get a player input
        char *buffer = get_input();

        // Quit a game
        if (strcmp(buffer, "q") == 0) {
            break;
        }

        int x, y;
        if (!parse_input(&x, &y, buffer)) {
            continue;
        }

        if (!is_valid_move(game.next, x, y)) {
            printf("Invalid move\n");
            continue;
        }

        put_disk(game.next, x, y);

        next_turn();
    }

    return !game.playable[0] && !game.playable[1];
}

// Judge a game
void judge_game(void) {
    print_board(game.next);

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
