#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

void parse_args(const int argc, const char *argv[]);

void init_game(void);

bool play_game(void);

void judge_game(void);

#endif
