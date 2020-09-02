#ifndef _PRESENTATION_INPUT_H_
#define _PRESENTATION_INPUT_H_

#include <stdbool.h>
#include <stdint.h>

#include "SDL.h"

#include "../domain/Input.h"

void read_input(Input *input,
        const int8_t kb_left, const int8_t kb_right, const bool kb_a_button, SDL_GameController *controller);
#endif // _PRESENTATION_INPUT_H_
