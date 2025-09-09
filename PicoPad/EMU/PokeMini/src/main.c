#include "main.h"
#include "PokeMini.h"
#include "Hardware.h"

// Basic entry point wiring the PokeMini core
int main(void)
{
    // Initialize emulator with no special flags and no sound buffer
    if (!PokeMini_Create(0, 0))
        return 1;

    // Run dummy frames forever; ROM loading and I/O will be added later
    while (1)
        PokeMini_EmulateFrame();

    return 0;
}
