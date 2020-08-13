#ifndef _GAME_H_
#define _GAME_H_

#include "SDL.h"

#define WHITE 0xffffffff

typedef struct {
    float x;
    float y;
} Player;

typedef struct {
    Uint32 nb_ticks;
    Uint32 w;
    Uint32 h;
    Player player;
} World;

typedef struct {
    Uint32 w;
    Uint32 h;
    Uint32 memory_size;
    Uint32 (*pixels)[];
} Framebuffer;

void init_world(World *world, Uint32 w, Uint32 h);
void init_framebuffer(Framebuffer *fb, Uint32 w, Uint32 h, Uint32 (*pixels)[]);
void update(World *world);
void draw(Framebuffer *fb, World const *world);
void draw_player(Framebuffer *fb, Player const *player);
void cls(Framebuffer *fb);

#endif  //  _GAME_H_
