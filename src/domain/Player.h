#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include <stdint.h>

#include "Input.h"

typedef struct Player *PlayerPtr;

PlayerPtr create_player(const double x, const double y);
void delete_player(PlayerPtr);

void update_player(PlayerPtr, const Input *input);

double player_x(const PlayerPtr);
double player_y(const PlayerPtr);

void player_clamp(const PlayerPtr, const double w, const double h);

#endif // _PLAYER_H_
