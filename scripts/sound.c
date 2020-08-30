#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "SDL.h"

#define SAMPLE_RATE 44100.
#define NB_CHANNELS 2
typedef int16_t T;
static const uint8_t BIT_DEPTH = 8*sizeof(T);

void audio_callback(void* userdata, Uint8* byte_stream, int len);
static T min_value();
static T max_value();

double sine_wave(const double frequency, const double t);
double saw_wave(const double frequency, const double t);
double pulse_wave(const double frequency, const double pulse_width, const double t);

typedef struct {
    double attack;
    double release;
} Envelope;

double apply_envelope(const Envelope *envelope, const double t, const double value);

int main() {
    if(0 != SDL_Init(SDL_INIT_AUDIO)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_AudioSpec want, have;
    SDL_AudioDeviceID dev;

    SDL_memset(&want, 0, sizeof(want));
    want.freq = SAMPLE_RATE;
    want.format = (8 == BIT_DEPTH) ? AUDIO_S8 : AUDIO_S16SYS;
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

void audio_callback(void* userdata, Uint8* byte_stream, int len) {
    (void)userdata; // unused variable
    static const double SINE_FREQ = 220.;
    static const double SAW_FREQ = 1.001*3*220.;
    static const double PULSE_FREQ = 0.999*5*55.;
    static const Envelope envelope = {
        .attack = 0.01,
        .release = 0.8,
    };
    static int start_frame = 0;

    const int nb_frames = len/(NB_CHANNELS*sizeof(T));
    T *stream = (T *)byte_stream;

    for (int i = 0; i < nb_frames; ++i) {
        const double t = (start_frame + i)/SAMPLE_RATE;

        const double sine = sine_wave(SINE_FREQ, t);
        const double saw = saw_wave(SAW_FREQ, t);
        const double pulse = pulse_wave(PULSE_FREQ, .25, t);

        const double x = apply_envelope(&envelope, t, 0.05*saw + 0.2*sine + 0.04*pulse);

        const T value = max_value()*x;
        assert(min_value() <= value && value <= max_value());

        for (int j = 0; j < NB_CHANNELS; ++j) {
            stream[NB_CHANNELS*i + j] = value;
        }
    }

    start_frame += nb_frames;
}

static T min_value() {
    return -pow(2, BIT_DEPTH-1);
}

static T max_value() {
    return pow(2, BIT_DEPTH-1)-1;
}

double sine_wave(const double frequency, const double t) {
    static const double TAU = 2*M_PI;
    return sin(t*TAU*frequency);
}

double saw_wave(const double frequency, const double t) {
    return 2*frequency*fmod(t, 1/frequency) - 1;
}

double pulse_wave(const double frequency, const double pulse_width, const double t) {
    return (fmod(t, 1/frequency) > pulse_width/frequency) ? 1 : 0;
}

double apply_envelope(const Envelope *envelope, const double t, const double value) {
    assert(0 <= t);

    if (0 < envelope->attack && t <= envelope->attack) {
        return value*t/envelope->attack;
    }

    const double duration = envelope->attack + envelope->release;

    if (0 == envelope->release || duration <= t) {
        return 0;
    }

    const double k = 1 + (envelope->attack-t)/envelope->release;

    return value*k;
}
