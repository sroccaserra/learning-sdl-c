#include <math.h>
#include <stdio.h>

#include "../src/domain/poly_sin_cos.h"

#include "../src/get_time_ms.h"

volatile double s, a;

int main() {
    double start_time;
    static const int nb_tries = 100000000;

    a = 0.5;

    start_time = get_time_ms();
    for(int i = 0; i < nb_tries; ++i) {
        s = sin(a);
    }

    printf("math.h sin (ms): %f\n", get_time_ms() - start_time);

    start_time = get_time_ms();
    for(int i = 0; i < nb_tries; ++i) {
        s = poly_sin(a);
    }

    printf("polynome sin (ms): %f\n", get_time_ms() - start_time);
}
