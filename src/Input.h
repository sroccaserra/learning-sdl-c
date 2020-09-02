#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int8_t x_direction;
    bool is_jumping;
} Input;

#endif // _INPUT_H_
