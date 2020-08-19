#ifndef _INIT_H_
#define _INIT_H_

#include <stdbool.h>

#include "SDL.h"

typedef enum {
    STATUS_SUCCESS,
    STATUS_ERROR
} ReturnStatus;

typedef struct {
    Uint32 nb_frames;
    double frame_average_ms;
    double total_time_ms;
} FrameStatistics;

typedef struct {
    int w;
    int h;
    int pixel_size;
    bool is_full_screen;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *low_res_screen;
    SDL_Texture *sprite_tiles;

    SDL_GameController *controller;

    FrameStatistics stats;
} Context;

void init_context(Context *context, int w, int h, int pixel_size);
void clean_context(Context *context);

ReturnStatus init_window_renderer_and_screen_texture(Context *context);
ReturnStatus init_sprite_tiles(ReturnStatus previous, Context *context);
ReturnStatus init_controller(ReturnStatus previous, Context *context);

#endif // _INIT_H_
