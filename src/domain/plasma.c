#include <math.h>
#include <stdint.h>

#include "poly_sin_cos.h"
#include "plasma.h"

void draw_plasma(uint32_t (*pixels)[], uint32_t w, uint32_t h, double t) {
    for (uint32_t y = 0; y < h; ++y) {
        const int32_t y_offset = y*w;
        double ry = (double)y/h - 0.5;
        for (uint32_t x = 0; x < w; ++x) {
            double rx = (double)x/w - 0.5;
            (*pixels)[y_offset + x] = color_plasma(rx, ry, t);
        }
    }
}

uint32_t color_plasma(double rx, double ry, double t) {
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

    uint8_t r = 255*(0.5 + 0.5*poly_sin(v*M_PI));
    uint8_t g = 255*(0.5 + 0.5*poly_sin(v*M_PI + 2.*M_PI/3));
    uint8_t b = 255*(0.5 + 0.5*poly_sin(v*M_PI + 4.*M_PI/3));

    return (r << 24) + (g << 16) + (b << 8) + 0xff;
}
