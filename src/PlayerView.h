#ifndef _PLAYER_VIEW_H_
#define _PLAYER_VIEW_H_

#include "Panel.h"
#include "Player.h"

typedef struct {
    Player *player;
    Panel *panel;
} PlayerView;

void update_player_view(PlayerView *player_view, const Player *player);
int draw_player(SDL_Renderer *renderer, const PlayerView *player_view);

#endif // _PLAYER_VIEW_H_
