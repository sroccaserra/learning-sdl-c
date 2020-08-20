#include <stdbool.h>

#include "PresentationContext.h"

void init_presentation_context(PresentationContext *context, int w, int h, int pixel_size) {
    context->w = w;
    context->h = h;
    context->pixel_size = pixel_size;
    context->is_full_screen = false;

    context->window = NULL;
    context->renderer = NULL;
    context->sprite_tiles = NULL;

    context->controller = NULL;
}

void clean_presentation_context(PresentationContext *context) {
    if (NULL != context->controller) {
        SDL_GameControllerClose(context->controller);
    }
    if (NULL != context->sprite_tiles) {
        SDL_DestroyTexture(context->sprite_tiles);
    }
    if (NULL != context->renderer) {
        SDL_DestroyRenderer(context->renderer);
    }
    if (NULL != context->window) {
        SDL_DestroyWindow(context->window);
    }

    SDL_Quit();
}
