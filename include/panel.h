#ifndef _PANEL_H_
#define _PANEL_H_

#include "SDL.h"

typedef struct {
    SDL_Rect tileset_position;
    double x;
    double y;
    double x_zoom;
    double y_zoom;
    double alpha;
    SDL_Point rotation_center;
    SDL_Texture *tileset;
} Panel;

int draw_panel(SDL_Renderer *renderer, Panel *panel);

#endif // _PANEL_H_
