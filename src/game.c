#include "game.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "com.h"

// Game data
typedef struct {
    Disk player;
    Disk current;
    int num_valid_moves[3];
    int valid_moves[3][60];
    bool record_game;
    int record[60];
    Board board;
} Game;

// Check equality of two strings
static inline bool str_eq(const char *s1, const char *s2) {
    return (strcmp(s1, s2) == 0);
}

static char *help_str =
    "usage: %s [-b] [-w] [-c] [-r] [-h]\n"
    "options:\n"
    "    -b: set player's color to black (default)\n"
    "    -w: set player's color to white\n"
    "    -c: COM vs COM mode\n"
    "    -r: output a game record (as './record.txt')\n"
    "    -h: show this help\n";

// Parse commandline arguments
static void parse_args(Game *game, const int argc, const char *argv[]) {
    game->player = BLACK;
    game->record_game = false;

    for (int i = 1; i < argc; ++i) {
        if (str_eq("-b", argv[i])) {
            game->player = BLACK;
        } else if (str_eq("-w", argv[i])) {
            game->player = WHITE;
        } else if (str_eq("-c", argv[i])) {
            game->player = NONE;
        } else if (str_eq("-r", argv[i])) {
            game->record_game = true;
        } else if (str_eq("-h", argv[i])) {
            printf(help_str, argv[0]);
            exit(EXIT_SUCCESS);
        }
    }
}

// Initialize the game
static void init_game(Game *game) {
    init_board(&(game->board));

    game->current = BLACK;

    srand(time(NULL));
}

// Check whether an input string is valid as a coordinate
static bool is_valid_input(const char *buffer) {
    if (strlen(buffer) == 2) {
        if (isalpha(buffer[0]) &&
            ((buffer[0] < 'i') || ((buffer[0] > 'z') && (buffer[0] < 'I'))) &&
            isdigit(buffer[1]) && (buffer[1] > '0') && (buffer[1] < '9')) {
            return true;
        }
    }

    return false;
}

// Clear the current line
static void clear_current_line(void) { printf("\r\033[0K"); }

// Get a player input
static char *get_input(const Disk turn) {
    static char buffer[3 + 1];

    clear_current_line();
    printf("%s turn > ", get_disk_str(turn));

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

    return buffer;
}

// Rewind the cursor
static void rewind_cursor(void) { printf("\r\033[12A"); }

// Judge a game
static void judge_game(const Game *game) {
    puts("");
    clear_current_line();

    int num_black = count_disks(&(game->board), BLACK);
    int num_white = count_disks(&(game->board), WHITE);

    printf("B:W=%d:%d\n", num_black, num_white);
    if (num_black > num_white) {
        printf("%s wins!\n", get_disk_str(BLACK));
    } else if (num_white > num_black) {
        printf("%s wins!\n", get_disk_str(WHITE));
    } else {
        printf("Draw\n");
    }
}

// Change a turn
static void change_turn(Game *game) {
    game->current = get_opposite(game->current);
}

// Back the line
static void back_a_line(void) { printf("\033[1A\r"); }

// Update valid moves
static void update_valid_moves(Game *game) {
    game->num_valid_moves[BLACK] =
        get_valid_moves(game->valid_moves[BLACK], &(game->board), BLACK);
    game->num_valid_moves[WHITE] =
        get_valid_moves(game->valid_moves[WHITE], &(game->board), WHITE);
}

void play_game(const int argc, const char *argv[]) {
    Game game;

    parse_args(&game, argc, argv);

    init_game(&game);

    int turn = 0;

    while (true) {
        update_valid_moves(&game);

        print_board(&(game.board), game.current);

        if (game.num_valid_moves[game.current] == 0) {
            if (game.num_valid_moves[get_opposite(game.current)] == 0) {
                break;
            }

            clear_current_line();
            printf("%s turn > pass\n", get_disk_str(game.current));

            change_turn(&game);

            rewind_cursor();
            continue;
        }

        int index;
        if (game.current == game.player) {
            while (true) {
                char *buffer = get_input(game.current);

                clear_current_line();

                // Quit a game
                if (str_eq(buffer, "q")) {
                    printf("Quit the game\n");
                    return;
                }

                if (is_valid_input(buffer)) {
                    index = get_index_from_pos_str(buffer);

                    if (is_valid_move(&(game.board), game.current, index)) {
                        break;
                    }

                    printf("Invalid move: cannot put at \"%s\"", buffer);
                    back_a_line();

                    continue;
                }

                clear_current_line();

                printf(
                    "Invalid input: specify a coordinate as \"a1\"-\"h8\" "
                    "(\"A1\"-\"H8\")");
                back_a_line();
            }
        } else {
            index = get_com_next_move(&(game.board), game.current, 3);

            printf("\n%s turn > %s", get_disk_str(game.current),
                   get_pos_str(index));
        }

        put_disk(&(game.board), game.current, index);

        change_turn(&game);

        if (game.record_game) {
            game.record[turn] = index;
            ++turn;
        }

        rewind_cursor();
    }

    judge_game(&game);

    if (game.record_game) {
        FILE *fp = fopen("record.txt", "w");

        for (int i = 0; i < 60; ++i) {
            fprintf(fp, "%s", get_pos_str(game.record[i]));
        }

        fclose(fp);
    }
}
