#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "game.h"
#include "get_time_ms.h"

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
    Uint32 pixels[w*h];

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

    SDL_Texture *texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
            w, h);
    if (NULL == texture) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto TextureCreationFailed;
    }

    World world;
    init_world(&world, w, h);

    Framebuffer fb;
    init_framebuffer(&fb, w, h, &pixels);

    char *fullscreen_config = getenv("FULLSCREEN");
    if (NULL != fullscreen_config && 0 == strcmp("true", fullscreen_config)) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    SDL_Event event;
    bool quit = false;
    Uint32 nb_frames = 0;
    Uint32 frame_start_ms, frame_end_ms;
    Uint32 frame_measure_start = 100;
    Uint32 nb_measured_frames = 5;
    double frame_average = 0;

    const double loop_start_time_ms = get_time_ms();

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
        update(&world);
        draw(&fb, &world);
        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_end_ms = SDL_GetTicks();
            frame_average += (double)(frame_end_ms - frame_start_ms)/nb_measured_frames;
        }

        int result = SDL_UpdateTexture(texture, NULL, fb.pixels, fb.w * sizeof(Uint32));
        result += SDL_RenderCopy(renderer, texture, NULL, NULL);
        if (0 != result) {
            goto Fail;
        }

        SDL_RenderPresent(renderer);
    }

    const double total_time_ms = get_time_ms() - loop_start_time_ms;

    printf("\nNb frames: %d\n", nb_frames);
    printf("Total time (ms): %f\n", total_time_ms);
    printf("Average FPS: %f\n", 1000.f*nb_frames/total_time_ms);
    printf("100th frame computing time (ms): %d\n", frame_end_ms - frame_start_ms);
    printf("Average frame computing time (ms): %f\n", frame_average);

    status = EXIT_SUCCESS;
Fail:
    SDL_DestroyTexture(texture);
TextureCreationFailed:
    SDL_DestroyRenderer(renderer);
RendererInitFailed:
    SDL_DestroyWindow(window);
WindowInitFailed:
    SDL_Quit();

    return status;
}
