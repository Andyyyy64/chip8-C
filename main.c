#include <stdio.h>

#include "SDL.h"


// SDL object
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AudioSpec want, hav;
    SDL_AudioDeviceID dev;
} sdl_t;

// emulator's statas
typedef enum {
    QUIT,
    RUNNING,
    PAUSED,
} emu_state_t;

// chip8 extension support enum
typedef enum {
    CHIP8,
    SUOERCHIP,
    XOCHIP,
} emu_type_t;
