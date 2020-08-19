#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "get_time_ms.h"
#include "panel.h"

static const double WALL_TEXTURE_X = 0;
static const double WALL_TEXTURE_Y = 0;
static const double WALL_TEXTURE_W = 16;
static const double WALL_TEXTURE_H = 16;

typedef struct {
    Uint32 nb_frames;
    double frame_average_ms;
    double total_time_ms;
} FrameStatistics;

FrameStatistics game_loop(SDL_Renderer *renderer, SDL_Texture *low_res_screen, int w, int h, SDL_Texture *sprite_tiles);

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    int status = EXIT_FAILURE;

    Uint32 w = 320;
    Uint32 h = 240;
    Uint32 pixel_size = 3;

    SDL_Window *window = SDL_CreateWindow("SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w*pixel_size, h*pixel_size, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        goto WindowInitFailed;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto RendererInitFailed;
    }

    char *fullscreen_config = getenv("FULLSCREEN");
    if (NULL != fullscreen_config && 0 == strcmp("true", fullscreen_config)) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    SDL_Texture *low_res_screen = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            w, h);

    SDL_Surface *sprite_tiles_surface = SDL_LoadBMP("assets/Sprite-0001.bmp");
    if (NULL == sprite_tiles_surface) {
        fprintf(stderr, "Erreur loading bmp: %s", SDL_GetError());
        goto TextureCreationFailed;
    }

    Uint32 *pixels = sprite_tiles_surface->pixels;
    SDL_SetColorKey(sprite_tiles_surface, SDL_TRUE, *pixels);

    SDL_Texture *sprite_tiles = SDL_CreateTextureFromSurface(renderer, sprite_tiles_surface);
    if (NULL == sprite_tiles) {
        fprintf(stderr, "Erreur loading bmp: %s", SDL_GetError());
        goto TextureCreationFailed;
    }

    SDL_FreeSurface(sprite_tiles_surface);

    const FrameStatistics stats = game_loop(renderer, low_res_screen, w, h, sprite_tiles);

    printf("\nNb frames: %d\n", stats.nb_frames);
    printf("Total time (ms): %f\n", stats.total_time_ms);
    printf("Average FPS: %f\n", 1000.f*stats.nb_frames/stats.total_time_ms);
    printf("Average frame computing time (ms): %f\n", stats.frame_average_ms);

    status = EXIT_SUCCESS;

    SDL_DestroyTexture(low_res_screen);
    SDL_DestroyTexture(sprite_tiles);
TextureCreationFailed:
    SDL_DestroyRenderer(renderer);
RendererInitFailed:
    SDL_DestroyWindow(window);
WindowInitFailed:
    SDL_Quit();

    return status;
}

FrameStatistics game_loop(SDL_Renderer *renderer, SDL_Texture *low_res_screen, int w, int h, SDL_Texture *sprite_tiles) {
    const double loop_start_time_ms = get_time_ms();

    const double zoom_factor = 3;
    const double size_x = zoom_factor*WALL_TEXTURE_W;
    const double size_y = zoom_factor*WALL_TEXTURE_H;
    Panel character_panel = {
        {WALL_TEXTURE_X, WALL_TEXTURE_Y, WALL_TEXTURE_W, WALL_TEXTURE_H},
        (w-size_x)/2, (h-size_y)/2,
        zoom_factor, zoom_factor,
        0, {size_x/2, size_y/2},
        sprite_tiles
    };

    SDL_Event event;
    bool quit = false;

    Uint32 nb_frames = 0;
    Uint32 frame_start_ms, frame_end_ms;
    Uint32 frame_measure_start = 100;
    Uint32 nb_measured_frames = 5;
    double frame_average = 0;

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
        if (character_panel.x > w) {
            character_panel.x -= w;
        }
        character_panel.alpha += 6*M_1_PI;

        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_end_ms = SDL_GetTicks();
            frame_average += (double)(frame_end_ms - frame_start_ms)/nb_measured_frames;
        }

        SDL_SetRenderTarget(renderer, low_res_screen);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        draw_panel(renderer, &character_panel);

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, low_res_screen, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    const double total_time_ms = get_time_ms() - loop_start_time_ms;

    const FrameStatistics stats = {
        nb_frames,
        frame_average,
        total_time_ms
    };
    return stats;
}
