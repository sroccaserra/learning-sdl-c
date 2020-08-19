#include "SDL.h"

#include "init.h"

ReturnStatus init_window_renderer_and_screen_texture(Context *context) {
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return STATUS_ERROR;
    }

    context->window = SDL_CreateWindow("SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            context->w*context->pixel_size, context->h*context->pixel_size, SDL_WINDOW_SHOWN);
    if(NULL == context->window)
    {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return STATUS_ERROR;
    }

    if (context->is_full_screen) {
        SDL_SetWindowFullscreen(context->window, SDL_WINDOW_FULLSCREEN);
    }

    context->renderer = SDL_CreateRenderer(context->window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(NULL == context->renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        return STATUS_ERROR;
    }

    context->low_res_screen = SDL_CreateTexture(context->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            context->w, context->h);
    if(NULL == context->low_res_screen) {
        fprintf(stderr, "SDL_CreateTexture error during low_res_screen init: %s\n", SDL_GetError());
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

ReturnStatus init_sprite_tiles(ReturnStatus previous, Context *context) {
    if (STATUS_SUCCESS != previous) {
        return previous;
    }

    const char *file_name = "assets/Sprite-0001.bmp";
    SDL_Surface *sprite_tiles_surface = SDL_LoadBMP(file_name);
    if (NULL == sprite_tiles_surface) {
        fprintf(stderr, "Error loading bmp file %s: %s\n", file_name, SDL_GetError());
        return STATUS_ERROR;
    }

    Uint32 *pixels = sprite_tiles_surface->pixels;
    SDL_SetColorKey(sprite_tiles_surface, SDL_TRUE, *pixels);

    context->sprite_tiles = SDL_CreateTextureFromSurface(context->renderer, sprite_tiles_surface);
    if (NULL == context->sprite_tiles) {
        fprintf(stderr, "Error creating sprite tiles texture: %s\n", SDL_GetError());
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

ReturnStatus init_controller(ReturnStatus previous, Context *context) {
    if (STATUS_SUCCESS != previous) {
        return previous;
    }

    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            printf("%d is a game controller\n", i);
            context->controller = SDL_GameControllerOpen(i);
            if (NULL == context->controller) {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
                return STATUS_ERROR;
            }
            return STATUS_SUCCESS;
        }
    }
    printf("No controller found!\n");

    return STATUS_SUCCESS;
}

