#include "game.h"

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    init_game();

    if (play_game()) {
        judge_game();
    }

    return 0;
}
