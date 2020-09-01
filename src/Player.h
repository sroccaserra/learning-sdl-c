#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    double x;
    double y;
    double v_x;
    double v_y;
} Player;

void init_player(Player *player, double x, double y);
void update_player(Player *player, int8_t input_x_direction, bool input_jump);

#endif // _PLAYER_H_
