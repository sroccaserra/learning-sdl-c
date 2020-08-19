#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "get_time_ms.h"
#include "panel.h"

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

    FrameStatistics stats;
} Context;

void init_context(Context *context, int w, int h, int pixel_size);
ReturnStatus init_window_renderer_and_screen_texture(Context *context);
ReturnStatus init_sprite_tiles(ReturnStatus previous, Context *context);
ReturnStatus run_game_loop(ReturnStatus previous, Context *context);
void print_stats(FrameStatistics stats);
void clean_context(Context *context);

int main()
{
    Context context;
    init_context(&context, 320, 240, 3);

    char *fullscreen_config = getenv("FULLSCREEN");
    context.is_full_screen = (NULL != fullscreen_config) && (0 == strcmp("true", fullscreen_config));

    ReturnStatus status = init_window_renderer_and_screen_texture(&context);
    status = init_sprite_tiles(status, &context);
    status = run_game_loop(status, &context);

    print_stats(context.stats);
    clean_context(&context);

    if (STATUS_SUCCESS != status) {
        return status;
    }

    return EXIT_SUCCESS;
}

void init_context(Context *context, int w, int h, int pixel_size) {
    context->w = w;
    context->h = h;
    context->pixel_size = pixel_size;
    context->is_full_screen = false;

    context->window = NULL;
    context->renderer = NULL;
    context->low_res_screen = NULL;
    context->sprite_tiles = NULL;
}

ReturnStatus init_window_renderer_and_screen_texture(Context *context) {
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
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

ReturnStatus run_game_loop(ReturnStatus previous, Context *context) {
    if (STATUS_SUCCESS != previous) {
        return previous;
    }

    const double loop_start_time_ms = get_time_ms();

    const double CHARACTER_TEXTURE_X = 0;
    const double CHARACTER_TEXTURE_Y = 0;
    const double CHARACTER_TEXTURE_W = 16;
    const double CHARACTER_TEXTURE_H = 16;

    const double zoom_factor = 3;
    const double size_x = zoom_factor*CHARACTER_TEXTURE_W;
    const double size_y = zoom_factor*CHARACTER_TEXTURE_H;
    Panel character_panel = {
        {CHARACTER_TEXTURE_X, CHARACTER_TEXTURE_Y, CHARACTER_TEXTURE_W, CHARACTER_TEXTURE_H},
        (context->w-size_x)/2, (context->h-size_y)/2,
        zoom_factor, zoom_factor,
        0, {size_x/2, size_y/2},
        context->sprite_tiles
    };

    SDL_Event event;
    bool quit = false;

    Uint32 nb_frames = 0;
    Uint32 frame_start_ms, frame_end_ms;
    Uint32 frame_measure_start = 100;
    Uint32 nb_measured_frames = 5;
    double frame_average_ms = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        nb_frames += 1;
        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_start_ms = SDL_GetTicks();
        }

        character_panel.x += 1;
        if (character_panel.x > context->w) {
            character_panel.x -= context->w;
        }
        character_panel.alpha += 6*M_1_PI;

        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_end_ms = SDL_GetTicks();
            frame_average_ms += (double)(frame_end_ms - frame_start_ms)/nb_measured_frames;
        }

        SDL_SetRenderTarget(context->renderer, context->low_res_screen);
        SDL_SetRenderDrawColor(context->renderer, 0x39, 0x39, 0x39, 0x39);
        SDL_RenderClear(context->renderer);
        draw_panel(context->renderer, &character_panel);

        SDL_SetRenderTarget(context->renderer, NULL);
        SDL_RenderCopy(context->renderer, context->low_res_screen, NULL, NULL);

        SDL_RenderPresent(context->renderer);
    }

    const double total_time_ms = get_time_ms() - loop_start_time_ms;

    context->stats.nb_frames = nb_frames;
    context->stats.frame_average_ms = frame_average_ms;
    context->stats.total_time_ms = total_time_ms;

    return STATUS_SUCCESS;
}

void print_stats(FrameStatistics stats) {
    printf("\nNb frames: %d\n", stats.nb_frames);
    printf("Total time (ms): %f\n", stats.total_time_ms);
    printf("Average FPS: %f\n", 1000.f*stats.nb_frames/stats.total_time_ms);
    printf("Average frame computing time (ms): %f\n", stats.frame_average_ms);
}

void clean_context(Context *context) {
    if (NULL != context->sprite_tiles) {
        SDL_DestroyTexture(context->sprite_tiles);
    }
    if (NULL != context->low_res_screen) {
        SDL_DestroyTexture(context->low_res_screen);
    }
    if (NULL != context->renderer) {
        SDL_DestroyRenderer(context->renderer);
    }
    if (NULL != context->window) {
        SDL_DestroyWindow(context->window);
    }

    SDL_Quit();
}
