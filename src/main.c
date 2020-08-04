#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef struct {
    float x;
    float y;
} Player;

typedef struct {
    int nb_ticks;
    Player player;
} World;

void update(World *world);
int draw(SDL_Renderer *renderer, World *world);

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    int status = EXIT_FAILURE;

    SDL_Window *window = SDL_CreateWindow("SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        goto WindowInitFailed;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto RendererInitFailed;
    }

    World world = {0, {10, 10}};

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        update(&world);
        if(0 != draw(renderer, &world)) {
            goto Fail;
        }
    }

    status = EXIT_SUCCESS;
Fail:
    SDL_DestroyRenderer(renderer);
RendererInitFailed:
    SDL_DestroyWindow(window);
WindowInitFailed:
    SDL_Quit();

    return status;
}

void update(World *world) {
    world->nb_ticks += 1;
    world->player.x += 1;
}

int draw(SDL_Renderer *renderer, World *world) {
    int result = 0;

    SDL_Color background_color = {63, 63, 63, 255};
    result += SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    result += SDL_RenderClear(renderer);

    SDL_Color green = {40, 255, 255, 255};
    result += SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
    Player *player = &world->player;
    SDL_Rect rect = {round(player->x), round(player-> y), 10, 10};
    result += SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

    return result;
}
