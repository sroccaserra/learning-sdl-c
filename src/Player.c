#include "Player.h"

void init_player(Player *player, double x, double y) {
    player->x = x;
    player->y = y;
}

void update_player(Player *player, int input_x_direction, int input_y_direction) {
    player->x += input_x_direction;
    player->y += input_y_direction;
}
