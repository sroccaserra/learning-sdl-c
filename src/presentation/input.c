#include "input.h"

void read_input(Input *input,
        const int8_t kb_left, const int8_t kb_right, const bool kb_a_button, SDL_GameController *controller) {
    int8_t kb_x_direction = kb_left + kb_right;

    int8_t input_x_direction = 0;

    if (kb_x_direction == 1 ||
            1 == SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        input_x_direction = 1;
    }
    if (kb_x_direction == -1 ||
            1 == SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        input_x_direction = -1;
    }

    int8_t input_jump = false;
    if (kb_a_button || 1 == SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
        input_jump = true;
    }

    input->x_direction = input_x_direction;
    input->is_jumping = input_jump;
}
