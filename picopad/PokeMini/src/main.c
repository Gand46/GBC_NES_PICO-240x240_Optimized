#include <string.h>
#include "PokeMini.h"
#include "Hardware.h"

// Dummy ROM placeholder embedded into firmware
static const uint8_t dummy_rom[0x2000] = {0};

int main(void)
{
    PokeMini_Create(0, 0);
    PokeMini_NewMIN(sizeof(dummy_rom));
    memcpy(PM_ROM, dummy_rom, sizeof(dummy_rom));
    PokeMini_Reset(0);

    while (1) {
        PokeMini_EmulateFrame();
    }
    return 0;
}
