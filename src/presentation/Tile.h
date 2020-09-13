#ifndef _TILE_H_
#define _TILE_H_

#include "SDL.h"

typedef struct {
    int tileset_x;
    int tileset_y;
    int tileset_w;
    int tileset_h;

    double x;
    double y;
    double x_zoom;
    double y_zoom;
    double alpha;
    SDL_Point rotation_center;
    SDL_Texture *tileset;
} Tile;

int draw_tile(SDL_Renderer *renderer, const Tile *tile);

#endif // _TILE_H_
