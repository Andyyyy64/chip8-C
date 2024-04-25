#include "cpu.h"
#include "font.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void initialize_state(struct StateChip8 *state) {
    memset(state, 0, sizeof(struct StateChip8));
    state->PC = PROGRAM_OFFSET;
    load_font(state);
}

void load_rom(struct StateChip8 *state, uint8_t *rom, size_t rom_size) {

}

void load_font(struct StateChip8 *state) {
    uint8_t *font = font_gen(); // generate the font
    memcpy(state->memory + FONT_OFFSET, font, FONT_SIZE); // copy the font to the memory(memcpy-> dist, src, size)
}

void update(struct StateChip8 *state) {
    if(state->delay_timer > 0) {
        state->delay_timer--;
    }
    if(state->sound_timer > 0) {
        state->sound_timer--;
    }
}

void draw(struct StateChip8 *state, uint16_t opcode, uint8_t vx, uint8_t vy) {

}

void emulate_op(struct StateChip8 *state) {

}
