#include "game.h"

int main(void) {
    init_game();

    if (play_game()) {
        judge_game();
    }

    return 0;
}
