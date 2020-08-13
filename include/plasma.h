#include <stdint.h>

#include "SDL2/SDL.h"

void draw_plasma(Uint32 (*pixels)[], Uint32 w, Uint32 h, double t);
uint32_t color_plasma(double rx, double ry, double t);
