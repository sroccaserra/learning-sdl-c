#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct {
    double x;
    double y;
} Player;

void init_player(Player *player, double x, double y);
void update_player(Player *player, int input_x_direction, int input_y_direction);

#endif // _PLAYER_H_
