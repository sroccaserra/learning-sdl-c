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

typedef struct {
    int w;
    int h;
    Uint32 (*pixels)[];
} Framebuffer;

void init(World *world, int w, int h);
void update(World *world);
void draw(Framebuffer *fb, World const *world);
void draw_player(Framebuffer *fb, Player const *player);
void cls(Framebuffer *fb);

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    int status = EXIT_FAILURE;

    int w = 320;
    int h = 240;
    int pixel_size = 3;
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
    init(&world, w, h);
    Framebuffer fb;
    fb.w = w;
    fb.h = h;
    fb.pixels = &pixels;

    SDL_Event event;
    bool quit = false;
    while (!quit) {
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

void cls(Framebuffer *fb) {
    memset(fb->pixels, 63, (fb->w*fb->h)*sizeof(Uint32));
}

void draw(Framebuffer *fb, World const *world) {
    cls(fb);

    Player const *player = &world->player;
    draw_player(fb, player);
}

void draw_player(Framebuffer *fb, Player const *player) {
    int x = round(player->x);
    int y = round(player->y);
    int player_color = 0xffffffff;

    (*fb->pixels)[y*fb->w + x] = player_color;
}
