#include "main.h"
#include "PokeMini.h"
#include "Hardware.h"
#include <stdio.h>

#define SAMPLE_ROM_PATH "samples/sample.min"

static int load_rom(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (!PokeMini_NewMIN(size)) {
        fclose(f);
        return 0;
    }

    if (fread(PM_ROM, 1, size, f) != (size_t)size) {
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}

// Basic entry point wiring the PokeMini core
int main(void)
{
    if (!PokeMini_Create(0, 0))
        return 1;

    if (!load_rom(SAMPLE_ROM_PATH))
        return 1;

    PokeMini_Reset(1);

    while (1)
        PokeMini_EmulateFrame();

    return 0;
}
