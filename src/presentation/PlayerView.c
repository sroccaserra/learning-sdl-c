#include "PlayerView.h"

static const double CHARACTER_TEXTURE_X = 0;
static const double CHARACTER_TEXTURE_Y = 0;
static const double CHARACTER_TEXTURE_W = 16;
static const double CHARACTER_TEXTURE_H = 16;

void init_player_view(PlayerView *player_view, const PlayerPtr player_p, SDL_Texture *tileset) {
    const double zoom_factor = 1;
    const double size_x = zoom_factor*CHARACTER_TEXTURE_W;
    const double size_y = zoom_factor*CHARACTER_TEXTURE_H;

    player_view->player_p = player_p;

    Tile *tile = &player_view->tile;
    tile->tileset_x = CHARACTER_TEXTURE_X;
    tile->tileset_y = CHARACTER_TEXTURE_Y;
    tile->tileset_w = CHARACTER_TEXTURE_W;
    tile->tileset_h = CHARACTER_TEXTURE_H;

    tile->x = player_x(player_p) - size_x/2.;
    tile->y = player_y(player_p) - size_y/2.;
    tile->x_zoom = zoom_factor;
    tile->y_zoom = zoom_factor;
    tile->rotation_center.x = size_x/2;
    tile->rotation_center.y = size_y/2;
    tile->tileset = tileset;
}

void update_player_view(PlayerView *player_view) {
    Tile *tile = &player_view->tile;

    const double size_x = tile->x_zoom*tile->tileset_w;
    const double size_y = tile->y_zoom*tile->tileset_h;

    tile->x = player_x(player_view->player_p) - size_x/2.;
    tile->y = player_y(player_view->player_p) - size_y/2.;
}

int draw_player_view(SDL_Renderer *renderer, const PlayerView *player_view) {
    return draw_tile(renderer, &player_view->tile);
}
