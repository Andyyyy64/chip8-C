#include "state.h"
#include <stddef.h>

void load_rom(struct StateChip8 *state, uint8_t *rom, size_t rom_size);
void load_font(struct StateChip8 *state);
void initalize_state(struct StateChip8 *state);
void emulate_op(struct StateChip8 *state);
void update_emu(struct StateChip8 *state);
