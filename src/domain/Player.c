#include <stdlib.h>

#include "Player.h"

struct Player {
    double x;
    double y;
    double v_x;
    double v_y;
};

void init_player(PlayerPtr player_p, const double x, const double y) {
    player_p->x = x;
    player_p->y = y;
    player_p->v_x = 0;
    player_p->v_y = 0;
}

PlayerPtr create_player(const double x, const double y) {
    const PlayerPtr player_p = malloc(sizeof *player_p);
    init_player(player_p, x, y);
    return player_p;
}

void delete_player(PlayerPtr player_p) {
    free(player_p);
}

void update_player(PlayerPtr player_p, const Input *input) {
    player_p->v_x = 2*input->x_direction;

    if (player_p->y < 200) {
        player_p->v_y += 0.5;
    }
    else {
        player_p->v_y = 0;
        player_p->y = 200;
    }

    if (input->is_jumping && player_p->y == 200) {
        player_p->v_y = -10;
    }

    player_p->x += player_p->v_x;
    player_p->y += player_p->v_y;
}

double player_x(const PlayerPtr player_p) {
    return player_p->x;
}

double player_y(const PlayerPtr player_p) {
    return player_p->y;
}

void player_clamp(const PlayerPtr player_p, const double w, const double h) {
    if (player_p->x > w) {
        player_p->x -= w;
    }
    if (player_p->x < 0) {
        player_p->x += w;
    }
    if (player_p->y < 0) {
        player_p->y += h;
    }
    if (player_p->y > h) {
        player_p->y -= h;
    }
}
