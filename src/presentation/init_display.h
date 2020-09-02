#ifndef _INIT_H_
#define _INIT_H_

#include <stdbool.h>

#include "SDL.h"

#include "PresentationContext.h"

typedef enum {
    STATUS_SUCCESS,
    STATUS_ERROR
} ReturnStatus;

ReturnStatus init_window_renderer_and_screen_texture(PresentationContext *context);
ReturnStatus init_sprite_tiles(ReturnStatus previous, PresentationContext *context);
ReturnStatus init_controller(ReturnStatus previous, PresentationContext *context);

#endif // _INIT_H_
