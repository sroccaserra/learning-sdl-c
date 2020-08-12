#include <time.h>

double get_time_ms() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return 1000*t.tv_sec + t.tv_nsec/1000000.;
}
