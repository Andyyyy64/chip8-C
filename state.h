#pragma once
#include <stdint.h>

#define V_REGISTER_COUNT 16
#define STACK_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT
#define MEMORY_SIZE 4096
#define KEY_COUNT 16
#define FONT_OFFSET 0x10
#define PROGRAM_OFFSET 0x200

typedef struct StateChip8 {
    uint8_t memory[MEMORY_SIZE];
    uint16_t PC; // program counter
    uint16_t I; // index register
    uint8_t V[V_REGISTER_COUNT]; // registers
    uint16_t stack[STACK_SIZE];
    uint8_t SP; // stack pointer
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint8_t display[DISPLAY_SIZE];
    int draw_flag;
    uint8_t keys[KEY_COUNT];
} SatteChip8;
