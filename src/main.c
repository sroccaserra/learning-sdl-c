#include <SDL2/SDL.h>

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        return EXIT_FAILURE;

    SDL_Quit();

    return EXIT_SUCCESS;
}
