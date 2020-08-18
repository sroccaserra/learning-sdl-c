#include "SDL.h"

#include "panel.h"

int draw_panel(SDL_Renderer *renderer, Panel *panel) {
    SDL_Rect dst_rect;
    dst_rect.x = panel->x;
    dst_rect.y = panel->y;
    dst_rect.w = panel->x_zoom*panel->tileset_position.w;
    dst_rect.h = panel->y_zoom*panel->tileset_position.h;

   return SDL_RenderCopyEx(renderer, panel->tileset, &panel->tileset_position, &dst_rect, panel->alpha, &panel->rotation_center, SDL_FLIP_NONE);
}
