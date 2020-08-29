#include <stdio.h>
#include <math.h>

#include "SDL.h"

#define SAMPLE_RATE 44100.
#define NB_CHANNELS 2

void audio_callback(void* userdata, Uint8* stream, int len) {
    (void)userdata; // unused variable
    static const double SINE_FREQ = 440.;
    static const double SAW_FREQ = SINE_FREQ/8.;
    static int start_frame = 0;

    const int nb_frames = len/NB_CHANNELS;

    for (int i = 0; i < nb_frames; ++i) {
        const double t = (start_frame + i)/SAMPLE_RATE;

        const double sine = sin(t*2*M_PI*SINE_FREQ);
        const double saw = 2*SAW_FREQ*fmod(t, 1/SAW_FREQ) - 1;

        const int value = 30*saw + 60*sine;

        for (int j = 0; j < NB_CHANNELS; ++j) {
            stream[NB_CHANNELS*i + j] = value;
        }
    }

    start_frame += nb_frames;
}

int main() {
    if(0 != SDL_Init(SDL_INIT_AUDIO)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_AudioSpec want, have;
    SDL_AudioDeviceID dev;

    SDL_memset(&want, 0, sizeof(want));
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_S8;
    want.channels = NB_CHANNELS;
    want.samples = 64;
    want.callback = audio_callback;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (dev == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    } else {
        if (have.format != want.format) {
            SDL_Log("We didn't get wanted audio format.");
        }

        SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
        SDL_Delay(1000);
        SDL_CloseAudioDevice(dev);
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}
