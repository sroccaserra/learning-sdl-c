#include "PlayerView.h"

void update_player_view(PlayerView *player_view, const Player *player) {
    Panel *panel = player_view->panel;

    const double size_x = panel->x_zoom*panel->tileset_w;
    const double size_y = panel->y_zoom*panel->tileset_h;

    panel->x = player->x - size_x/2.;
    panel->y = player->y - size_y/2.;
}

int draw_player(SDL_Renderer *renderer, const PlayerView *player_view) {
    return draw_panel(renderer, player_view->panel);
}
