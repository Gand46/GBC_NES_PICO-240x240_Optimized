#include "pico/stdlib.h"
#include <string.h>

#include "PokeMini.h"

// Dummy ROM placeholder embedded into firmware
static const uint8_t dummy_rom[0x2000] = {0};

int main(void) {
    stdio_init_all();

    // Initialize emulator and allocate ROM
    PokeMini_Create(0, 0);
    PokeMini_NewMIN(sizeof(dummy_rom));
    memcpy(PM_ROM, dummy_rom, sizeof(dummy_rom));
    PokeMini_Reset(0);

    // Main emulation loop
    while (true) {
        PokeMini_EmulateFrame();
    }

    return 0;
}
