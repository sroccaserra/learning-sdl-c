#include <math.h>

double poly_sin(double x) {
    static const double B = 4/M_PI;
    static const double C = -4/(M_PI*M_PI);
    static const double tau = 2*M_PI;

    x += M_PI;
    if (x < 0 || tau < x ) {
        x -= floor(x/tau)*tau;
    }
    x -= M_PI;

    return B * x + C * x * fabs(x);
}

double poly_cos(double x) {
    return poly_sin(x+M_PI_2);
}
