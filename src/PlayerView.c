#include "PlayerView.h"

static const double CHARACTER_TEXTURE_X = 0;
static const double CHARACTER_TEXTURE_Y = 0;
static const double CHARACTER_TEXTURE_W = 16;
static const double CHARACTER_TEXTURE_H = 16;

void init_player_view(PlayerView *player_view, const Player *player, SDL_Texture *tileset) {
    const double zoom_factor = 3;
    const double size_x = zoom_factor*CHARACTER_TEXTURE_W;
    const double size_y = zoom_factor*CHARACTER_TEXTURE_H;

    Panel *panel = &player_view->panel;
    panel->tileset_x = CHARACTER_TEXTURE_X;
    panel->tileset_y = CHARACTER_TEXTURE_Y;
    panel->tileset_w = CHARACTER_TEXTURE_W;
    panel->tileset_h = CHARACTER_TEXTURE_H;

    panel->x = player->x - size_x/2.;
    panel->y = player->y - size_y/2.;
    panel->x_zoom = zoom_factor;
    panel->y_zoom = zoom_factor;
    panel->rotation_center.x = size_x/2;
    panel->rotation_center.y = size_y/2;
    panel->tileset = tileset;
}

void update_player_view(PlayerView *player_view, const Player *player) {
    Panel *panel = &player_view->panel;

    const double size_x = panel->x_zoom*panel->tileset_w;
    const double size_y = panel->y_zoom*panel->tileset_h;

    panel->x = player->x - size_x/2.;
    panel->y = player->y - size_y/2.;
}

int draw_player_view(SDL_Renderer *renderer, const PlayerView *player_view) {
    return draw_panel(renderer, &player_view->panel);
}
