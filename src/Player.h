#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include <stdint.h>

#include "Input.h"

typedef struct {
    double x;
    double y;
    double v_x;
    double v_y;
} Player;

void init_player(Player *player, const double x, const double y);
void update_player(Player *player,const Input *input);

#endif // _PLAYER_H_
