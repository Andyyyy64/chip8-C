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

void update_emu(struct StateChip8 *state) {
    if(state->delay_timer > 0) {
        state->delay_timer--;
    }
    if(state->sound_timer > 0) {
        state->sound_timer--;
    }
}

void draw(struct StateChip8 *state, uint16_t opcode, uint8_t vx, uint8_t vy) {
    uint8_t rx = state->V[vx]; // sprite x
    uint8_t ry = state->V[vy]; // sprite y
    uint8_t sprite_height = opcode & 0x000F;
    uint8_t sprite_row;
    state->V[0xF] = 0; // reset collision bit

    for(int y = 0; y < sprite_height; y++) {
        sprite_row = state->memory[state->I + y];
        for(int x = 0; x < 8; x++) {
            if((sprite_row & (0x80 >> x)) != 0) {
                int display_pixel_address = ((ry + y) * DISPLAY_WIDTH + rx + x) % DISPLAY_SIZE;
                if(state->display[display_pixel_address] == 1) {
                    state->V[0xF] = 1;
                }
                state->display[display_pixel_address] ^= 1;
            }
        }
    }
    state->draw_flag = 1;
}

void emulate_op(struct StateChip8 *state) {
    // fetch
    uint16_t opcode = state->memory[state->PC];
    opcode <<= 8;
    opcode |= state->memory[state->PC + 1];
    uint16_t pc_old = state->PC;
    state->PC += 2;

    // parse vx and vy
    uint8_t vx, vy;
    vx = (opcode & 0x0F00) >> 8;
    vy = (opcode & 0x00F0) >> 4;

    // decode the first 4 bits

}

















