#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

typedef struct {
    float x;
    float y;
} Player;

typedef struct {
    int nb_ticks;
    int w;
    int h;
    Player player;
} World;

void init(World *world, int w, int h);
void update(World *world);
int draw(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 pixels[], World const *world);

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    int status = EXIT_FAILURE;
    int w = 320;
    int h = 240;

    Uint32 pixels[w*h];
    memset(pixels, 63, sizeof(pixels));

    SDL_Window *window = SDL_CreateWindow("SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w*2, h*2, SDL_WINDOW_SHOWN);
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
    init(&world, w, h);

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        update(&world);
        if(0 != draw(renderer, texture, pixels, &world)) {
            goto Fail;
        }
    }

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

void init(World *world, int w, int h) {
    world->nb_ticks = 0;
    world->w = w;
    world->h = h;

    Player *player = &world->player;
    player->x = 0;
    player->y = 0;
}

void update(World *world) {
    world->nb_ticks += 1;

    float center_x = world->w/2;
    float center_y = world->h/2;

    float r = 0.4*world->h;
    float alpha = world->nb_ticks/50.f;

    world->player.x = center_x + r*cos(alpha);
    world->player.y = center_y + r*sin(alpha);
}

int draw(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 pixels[], World const *world) {
    int result = 0;

    SDL_Color background_color = {63, 63, 63, 255};
    result += SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    result += SDL_RenderClear(renderer);

    SDL_UpdateTexture(texture, NULL, pixels, world->w * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_Color player_color = {40, 255, 255, 255};
    result += SDL_SetRenderDrawColor(renderer, player_color.r, player_color.g, player_color.b, player_color.a);
    Player const *player = &world->player;
    float size = 10;
    SDL_Rect rect = {round(player->x - size/2), round(player->y - size/2), size, size};
    result += SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

    return result;
}
