#ifndef _PRESENTATION_CONTEXT_H_
#define _PRESENTATION_CONTEXT_H_

#include <stdbool.h>
#include <stdint.h>

#include "SDL.h"

typedef struct {
    int w;
    int h;
    int pixel_size;
    bool is_full_screen;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprite_tiles;

    SDL_GameController *controller;

} PresentationContext;

void init_presentation_context(PresentationContext *context, int w, int h, int pixel_size);
void clean_presentation_context(PresentationContext *context);

#endif // _PRESENTATION_CONTEXT_H_
