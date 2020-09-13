#include "SDL.h"

#include "Tile.h"

int draw_tile(SDL_Renderer *renderer, const Tile *tile) {
    SDL_Rect src_rect;
    src_rect.x = tile->tileset_x;
    src_rect.y = tile->tileset_y;
    src_rect.w = tile->tileset_w;
    src_rect.h = tile->tileset_h;

    SDL_Rect dst_rect;
    dst_rect.x = tile->x;
    dst_rect.y = tile->y;
    dst_rect.w = tile->x_zoom*tile->tileset_w;
    dst_rect.h = tile->y_zoom*tile->tileset_h;

   return SDL_RenderCopyEx(
           renderer, tile->tileset, &src_rect, &dst_rect, tile->alpha, &tile->rotation_center, SDL_FLIP_NONE);
}
