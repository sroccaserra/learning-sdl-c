#include <math.h>
#include <string.h>

#include "game.h"

void init_world(World *world, Uint32 w, Uint32 h) {
    world->nb_ticks = 0;
    world->w = w;
    world->h = h;

    Player *player = &world->player;
    player->x = 0;
    player->y = 0;
}

void init_framebuffer(Framebuffer *fb, Uint32 w, Uint32 h, Uint32 (*pixels)[]) {
    fb->w = w;
    fb->h = h;
    fb->pixels = pixels;
    fb->memory_size = w*h*sizeof(Uint32);
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
    memset(fb->pixels, 63, fb->memory_size);
}

void draw(Framebuffer *fb, World const *world) {
    cls(fb);
    draw_player(fb, &world->player);
}

void draw_player(Framebuffer *fb, Player const *player) {
    Sint32 x = round(player->x);
    Sint32 y = round(player->y);
    Uint32 player_color = WHITE;

    (*fb->pixels)[y*fb->w + x] = player_color;
}
