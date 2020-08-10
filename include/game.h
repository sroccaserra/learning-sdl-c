#ifndef _GAME_H_
#define _GAME_H_

#include "SDL2/SDL.h"

#define WHITE 0xffffffff

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
    int memory_size;
    Uint32 (*pixels)[];
} Framebuffer;

void init_world(World *world, int w, int h);
void init_framebuffer(Framebuffer *fb, int w, int h, Uint32 (*pixels)[]);
void update(World *world);
void draw(Framebuffer *fb, World const *world);
void draw_player(Framebuffer *fb, Player const *player);
void cls(Framebuffer *fb);

#endif  //  _GAME_H_
