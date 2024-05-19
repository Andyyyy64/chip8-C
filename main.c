#include <SDL2/SDL.h>

#include <stdio.h>
#include "state.h"
#include "cpu.h"
#include "keymap.h"

#define SCREEN_W 640
#define SCREEN_H 320

static SDL_Surface *buffer = NULL;
static SDL_Surface *argb_buffer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Renderer *renderer;
static struct StateChip8 *state;

uint32_t total_cycles;

void draw_screen(SDL_Surface *surface, struct StateChip8 *state) {
    SDL_LockSurface(surface);

    uint32_t *pixels = (uint32_t *)surface->pixels; // get the pixel buffer
    for(int i = 0; i < DISPLAY_SIZE; i++) {
        pixels[i] = state->display[i] == 0 ? 0 : 0xFFFFFFFF; // set the pixel color
    }
    SDL_UnlockSurface(surface);
}

void update() {
    draw_screen(buffer, state);
    //surface to texture
    SDL_BlitScaled(buffer, NULL, argb_buffer, NULL);

    SDL_UpdateTexture(texture, NULL, argb_buffer->pixels, argb_buffer->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

uint8_t *read_testrom(size_t *rom_size) {
    char *name = "roms/test.ch8";
    FILE *file = fopen(name, "rb");
    if(!file) {
        fprintf(stderr, "Failed to open file %s\n", name);
        exit(1);
    }
    // get the file size
    fseek(file, 0 , SEEK_END); // fseek is used to move the file pointer to a specific location
    int size = (int)ftell(file); // ftell is the current position of the file pointer
    *rom_size = size;
    fseek(file, 0, SEEK_SET);

    uint8_t *rom_buffer = malloc(size);

    int read = fread(rom_buffer, 1, size, file);
    fclose(file);
    return rom_buffer;
}
void emu_cycle() {
    emulate_op(state);
    total_cycles++;

    if(total_cycles % 9 == 0) {
        update_emu(state);
    }

    usleep(1850);
}

int main(int argc, char *args[]) {
    state = (struct StateChip8 *)malloc(sizeof(struct StateChip8));
    initialize_state(state);
    size_t rom_size;
    uint8_t *rom = read_testrom(&rom_size);
    load_rom(state, rom, rom_size);
    free(rom);

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow(
        "emuchip8",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
    screenSurface = SDL_GetWindowSurface(window);

    //set up other surfaces
    buffer = SDL_CreateRGBSurface(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 32, 0, 0, 0, 0);
    argb_buffer = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, 32, 0, 0, 0, 0);

    while (1) {
        // Get the next event
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // Break out of the loop on quit
                break;
            }
            else if(event.type == SDL_KEYDOWN) {
                for(int i = 0; i < KEY_COUNT; i++) {
                    if(event.key.keysym.sym == KEYMAP[i]) {
                        state->keys[i] = 1;
                    }
                }
            }
            else if(event.type == SDL_KEYUP) {
                for(int i = 0; i < KEY_COUNT; i++) {
                    if(event.key.keysym.sym == KEYMAP[i]) {
                        state->keys[i] = 0;
                    }
                }
            }
        }
        //cycle the emulator
        emu_cycle();
        if (state->draw_flag)
        {
            state->draw_flag = 0;
            update();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
