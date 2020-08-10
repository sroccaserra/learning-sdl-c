#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"

#include "game.h"

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

    struct timespec loop_start_time, loop_end_time;
    clock_gettime(CLOCK_MONOTONIC, &loop_start_time);

    while (!quit) {
        nb_frames += 1;
        if (100 == nb_frames) {
            frame_start_ms = SDL_GetTicks();
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        update(&world);
        draw(&fb, &world);

        int result = SDL_UpdateTexture(texture, NULL, fb.pixels, fb.w * sizeof(Uint32));
        result += SDL_RenderCopy(renderer, texture, NULL, NULL);
        if (0 != result) {
            goto Fail;
        }

        SDL_RenderPresent(renderer);
        if (100 == nb_frames) {
            frame_end_ms = SDL_GetTicks();
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &loop_end_time);
    unsigned long long total_time_ms = 1000 * (loop_end_time.tv_sec - loop_start_time.tv_sec) + (loop_end_time.tv_nsec - loop_start_time.tv_nsec) / 1000000;

    printf("Nb frames: %d\n", nb_frames);
    printf("Total time (ms): %llu\n", total_time_ms);
    printf("Average FPS: %f\n", 1000.f*nb_frames/total_time_ms);
    printf("100th frame time (ms): %d\n", frame_end_ms - frame_start_ms);

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
