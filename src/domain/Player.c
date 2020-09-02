#include "Player.h"

void init_player(Player *player, const double x, const double y) {
    player->x = x;
    player->y = y;
    player->v_x = 0;
    player->v_y = 0;
}

void update_player(Player *player, const Input *input) {
    player->v_x = 2*input->x_direction;

    if (player->y < 200) {
        player->v_y += 0.5;
    }
    else {
        player->v_y = 0;
        player->y = 200;
    }

    if (input->is_jumping && player->y == 200) {
        player->v_y = -10;
    }

    player->x += player->v_x;
    player->y += player->v_y;
}