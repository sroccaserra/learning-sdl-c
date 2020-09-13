#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "domain/Input.h"
#include "domain/Player.h"

#include "presentation/Tile.h"
#include "presentation/PlayerView.h"
#include "presentation/PresentationContext.h"
#include "presentation/init_display.h"
#include "presentation/input.h"

#include "infrastructure/get_time_ms.h"

ReturnStatus run_game_loop(ReturnStatus previous, PresentationContext *context);

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

    clean_presentation_context(&context);

    if (STATUS_SUCCESS != status) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

ReturnStatus run_game_loop(ReturnStatus previous, PresentationContext *context) {
    if (STATUS_SUCCESS != previous) {
        return previous;
    }

    Player player;
    init_player(&player, context->w/2., context->h/2.);

    PlayerView player_view;
    init_player_view(&player_view, &player, context->sprite_tiles);

    SDL_Event event;
    bool quit = false;

    int8_t kb_left = 0;
    int8_t kb_right = 0;
    bool kb_a_button = false;

    const double FPS = 60.;
    const double wanted_frame_duration_ms = 1000./FPS;

    while (!quit) {
        const Uint64 frame_start_time_ms = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            kb_left = -1;
                            break;
                        case SDLK_RIGHT:
                            kb_right = 1;
                            break;
                        case SDLK_c:
                            kb_a_button = true;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            kb_left = 0;
                            break;
                        case SDLK_RIGHT:
                            kb_right = 0;
                            break;
                        case SDLK_c:
                            kb_a_button = false;
                            break;
                    }
                    break;
            }
        }

        Input input;
        read_input(&input, kb_left, kb_right, kb_a_button, context->controller);

        update_player(&player, &input);

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

        SDL_SetRenderDrawColor(context->renderer, 0x39, 0x39, 0x39, 0x39);
        SDL_RenderClear(context->renderer);

        update_player_view(&player_view, &player);
        draw_player_view(context->renderer, &player_view);

        SDL_RenderPresent(context->renderer);

        const double frame_duration_ms =
            1000.*(SDL_GetPerformanceCounter() - frame_start_time_ms)/SDL_GetPerformanceFrequency();

        if(frame_duration_ms > wanted_frame_duration_ms) {
            SDL_LogWarn(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "Frame duration = %.3f ms (wanted: %.3f ms)\n",
                    frame_duration_ms, wanted_frame_duration_ms);
        }
        if (wanted_frame_duration_ms > frame_duration_ms) {
            SDL_Delay(wanted_frame_duration_ms - frame_duration_ms);
        }
    }
    return STATUS_SUCCESS;
}
