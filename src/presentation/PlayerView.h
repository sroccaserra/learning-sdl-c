#ifndef _PLAYER_VIEW_H_
#define _PLAYER_VIEW_H_

#include "../domain/Player.h"

#include "Tile.h"

typedef struct {
    Tile tile;
    PlayerPtr player_p;
} PlayerView;

void init_player_view(PlayerView *player_view, const PlayerPtr, SDL_Texture *tileset);
int draw_player_view(SDL_Renderer *renderer, const PlayerView *player_view);

#endif // _PLAYER_VIEW_H_
