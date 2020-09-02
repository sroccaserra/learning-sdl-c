#ifndef _PLASMA_H_
#define _PLASMA_H_

#include <stdint.h>

void draw_plasma(uint32_t (*pixels)[], uint32_t w, uint32_t h, double t);
uint32_t color_plasma(double rx, double ry, double t);

#endif // _PLASMA_H_
