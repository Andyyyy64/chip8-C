#include <SDL2/SDL.h>

#include <stdio.h>
#include "state.h"
#include "cpu.h"
#include "keymap.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

static SDL_Surface *chip8buffer = NULL;
static SDL_Surface *argbbuffer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Renderer *renderer;
static struct StateChip8 *state;

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
    draw_screen(chip8buffer, state);
    //surface to texture
    SDL_BlitScaled(chip8buffer, NULL, argbbuffer, NULL);
    // Update the intermediate texture with the contents of the RGBA buffer.
    SDL_UpdateTexture(texture, NULL, argbbuffer->pixels, argbbuffer->pitch);
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

int main(int argc, char **argv) {

}
