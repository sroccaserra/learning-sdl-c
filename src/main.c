#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "get_time_ms.h"
#include "init.h"
#include "panel.h"

ReturnStatus run_game_loop(ReturnStatus previous, Context *context);
void print_stats(FrameStatistics stats);

int main()
{
    Context context;
    init_context(&context, 320, 240, 3);

    char *fullscreen_config = getenv("FULLSCREEN");
    context.is_full_screen = (NULL != fullscreen_config) && (0 == strcmp("true", fullscreen_config));

    ReturnStatus status = init_window_renderer_and_screen_texture(&context);
    status = init_sprite_tiles(status, &context);
    status = init_controller(status, &context);

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

    context->controller = NULL;
}

void clean_context(Context *context) {
    if (NULL != context->controller) {
        SDL_GameControllerClose(context->controller);
    }
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

    int kb_x_direction = 0;
    int kb_y_direction = 0;
    while (!quit) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            kb_x_direction = -1;
                            break;
                        case SDLK_RIGHT:
                            kb_x_direction = 1;
                            break;
                        case SDLK_UP:
                            kb_y_direction = -1;
                            break;
                        case SDLK_DOWN:
                            kb_y_direction = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (kb_x_direction < 0)
                                kb_x_direction = 0;
                            break;
                        case SDLK_RIGHT:
                            if (kb_x_direction > 0)
                                kb_x_direction = 0;
                            break;
                        case SDLK_UP:
                            if (kb_y_direction < 0)
                                kb_y_direction = 0;
                            break;
                        case SDLK_DOWN:
                            if (kb_y_direction > 0)
                                kb_y_direction = 0;
                            break;
                    }
                    break;
            }
        }

        nb_frames += 1;

        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_start_ms = SDL_GetTicks();
        }

        int x_direction = 0;
        int y_direction = 0;

        if (kb_x_direction == 1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
            x_direction = 1;
        }
        if (kb_x_direction == -1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
            x_direction = -1;
        }
        if (kb_y_direction == -1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
            y_direction = -1;
        }
        if (kb_y_direction == 1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
            y_direction = 1;
        }

        character_panel.x += x_direction;
        if (character_panel.x > context->w) {
            character_panel.x -= context->w;
        }
        if (character_panel.x < 0) {
            character_panel.x += context->w;
        }

        character_panel.y += y_direction;
        if (character_panel.y < 0) {
            character_panel.y += context->h;
        }
        if (character_panel.y > context->h) {
            character_panel.y -= context->h;
        }

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
