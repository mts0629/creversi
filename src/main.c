#include "game.h"

int main(const int argc, const char *argv[]) {
    parse_args(argc, argv);

    play_game();

    return 0;
}
