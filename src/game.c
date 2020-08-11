#include <math.h>
#include <string.h>

#include "game.h"
#include "poly_sin_cos.h"

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

Uint32 color_plasma(double rx, double ry, double t) {
    double rt = t/50.;

    double cx = rx + 0.5*poly_sin(rt/5);
    double cy = ry + 0.5*poly_cos(rt/3);

    double v =
        poly_sin(10.*rx + rt) +
        poly_sin((10.*ry + rt)/2.) +
        poly_sin((10.*(rx + ry) + rt)/2.) +
        poly_sin(10.*(rx*poly_sin(rt/2.) + ry*poly_cos(rt/3.)) + rt) +
        poly_sin(sqrt(100.*(cx*cx + cy*cy) + 1.) + rt);

    v = v/2.;

    int r = 255*(0.5 + 0.5*poly_sin(v*M_PI));
    int g = 255*(0.5 + 0.5*poly_sin(v*M_PI + 2.*M_PI/3));
    int b = 255*(0.5 + 0.5*poly_sin(v*M_PI + 4.*M_PI/3));

    return (r << 24) + (g << 16) + (b << 8) + 0xff;
}

void draw(Framebuffer *fb, World const *world) {
    for (Uint32 y = 0; y < world->h; ++y) {
        const Sint32 y_offset = y*fb->w;
        double ry = (double)y/fb->h - 0.5;
        for (Uint32 x = 0; x < world->w; ++x) {
            double rx = (double)x/fb->w - 0.5;
            (*fb->pixels)[y_offset + x] = color_plasma(rx, ry, world->nb_ticks);
        }
    }
}

void draw_player(Framebuffer *fb, Player const *player) {
    Sint32 x = round(player->x);
    Sint32 y = round(player->y);
    Uint32 player_color = WHITE;

    (*fb->pixels)[y*fb->w + x] = player_color;
}
