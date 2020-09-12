#include "SDL.h"

#include "Panel.h"

int draw_panel(SDL_Renderer *renderer, const Panel *panel) {
    SDL_Rect src_rect;
    src_rect.x = panel->tileset_x;
    src_rect.y = panel->tileset_y;
    src_rect.w = panel->tileset_w;
    src_rect.h = panel->tileset_h;

    SDL_Rect dst_rect;
    dst_rect.x = panel->x;
    dst_rect.y = panel->y;
    dst_rect.w = panel->x_zoom*panel->tileset_w;
    dst_rect.h = panel->y_zoom*panel->tileset_h;

   return SDL_RenderCopyEx(
           renderer, panel->tileset, &src_rect, &dst_rect, panel->alpha, &panel->rotation_center, SDL_FLIP_NONE);
}
