#ifndef _PLASMA_H_
#define _PLASMA_H_

#include <stdint.h>

#include "SDL.h"

void draw_plasma(Uint32 (*pixels)[], Uint32 w, Uint32 h, double t);
uint32_t color_plasma(double rx, double ry, double t);

#endif // _PLASMA_H_
