#include "../include.h"

void EmulatorLoop(void)
{
    // Create emulator instance and reset to a known state
    PokeMini_Create(0, 0);
    PokeMini_Reset(1);

    // Main execution loop
    while (true)
    {
        // Emulate a small slice of cycles
        PokeMini_EmulateCycles(1000);
    }
}

int main()
{
    EmulatorLoop();
    return 0;
}
