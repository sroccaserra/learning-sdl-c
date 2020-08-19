#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "get_time_ms.h"
#include "init_display.h"
#include "Panel.h"
#include "Player.h"
#include "PlayerView.h"
#include "PresentationContext.h"

ReturnStatus run_game_loop(ReturnStatus previous, PresentationContext *context);
void print_stats(FrameStatistics stats);

int main()
{
    PresentationContext context;
    init_presentation_context(&context, 320, 240, 3);

    char *fullscreen_config = getenv("FULLSCREEN");
    context.is_full_screen = (NULL != fullscreen_config) && (0 == strcmp("true", fullscreen_config));

    ReturnStatus status = init_window_renderer_and_screen_texture(&context);
    status = init_sprite_tiles(status, &context);
    status = init_controller(status, &context);

    status = run_game_loop(status, &context);

    print_stats(context.stats);
    clean_presentation_context(&context);

    if (STATUS_SUCCESS != status) {
        return status;
    }

    return EXIT_SUCCESS;
}

ReturnStatus run_game_loop(ReturnStatus previous, PresentationContext *context) {
    if (STATUS_SUCCESS != previous) {
        return previous;
    }

    const double loop_start_time_ms = get_time_ms();

    Player player;
    init_player(&player, context->w/2., context->h/2.);

    PlayerView player_view;
    init_player_view(&player_view, &player, context->sprite_tiles);

    SDL_Event event;
    bool quit = false;

    uint32_t nb_frames = 0;
    Uint32 frame_start_ms, frame_end_ms;
    uint32_t frame_measure_start = 100;
    int nb_measured_frames = 5;
    double frame_average_ms = 0;

    int kb_x_direction = 0;
    int kb_y_direction = 0;
    while (!quit) {
        nb_frames += 1;

        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_start_ms = SDL_GetTicks();
        }

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

        int input_x_direction = 0;
        int input_y_direction = 0;

        if (kb_x_direction == 1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
            input_x_direction = 1;
        }
        if (kb_x_direction == -1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
            input_x_direction = -1;
        }
        if (kb_y_direction == -1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
            input_y_direction = -1;
        }
        if (kb_y_direction == 1 || 1 == SDL_GameControllerGetButton(context->controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
            input_y_direction = 1;
        }

        update_player(&player, input_x_direction, input_y_direction);

        if (player.x > context->w) {
            player.x -= context->w;
        }
        if (player.x < 0) {
            player.x += context->w;
        }
        if (player.y < 0) {
            player.y += context->h;
        }
        if (player.y > context->h) {
            player.y -= context->h;
        }


        SDL_SetRenderTarget(context->renderer, context->low_res_screen);
        SDL_SetRenderDrawColor(context->renderer, 0x39, 0x39, 0x39, 0x39);
        SDL_RenderClear(context->renderer);

        update_player_view(&player_view, &player);
        draw_player_view(context->renderer, &player_view);

        SDL_SetRenderTarget(context->renderer, NULL);
        SDL_RenderCopy(context->renderer, context->low_res_screen, NULL, NULL);

        if (frame_measure_start <= nb_frames && nb_frames < frame_measure_start + nb_measured_frames) {
            frame_end_ms = SDL_GetTicks();
            frame_average_ms += (double)(frame_end_ms - frame_start_ms)/nb_measured_frames;
        }

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
