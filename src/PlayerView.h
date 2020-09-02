#ifndef _PLAYER_VIEW_H_
#define _PLAYER_VIEW_H_

#include "Panel.h"
#include "domain/Player.h"

typedef struct {
    Player *player;
    Panel panel;
} PlayerView;

void init_player_view(PlayerView *player_view, const Player *player, SDL_Texture *tileset);
void update_player_view(PlayerView *player_view, const Player *player);
int draw_player_view(SDL_Renderer *renderer, const PlayerView *player_view);

#endif // _PLAYER_VIEW_H_
