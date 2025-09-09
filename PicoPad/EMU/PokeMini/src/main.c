#include "../include.h"
#include <stdlib.h>

int main(void)
{
    DrawClear();
    DrawPrintText("PokeMini stub\n");

    if (!DiskMount())
    {
        DrawPrintText("No SD\n");
        while (1) { }
    }

    if (!SetDir("samples"))
    {
        DrawPrintText("No samples dir\n");
        while (1) { }
    }

    sFile rom;
    if (FileOpen(&rom, "sample.min"))
    {
        int size = FileSize(&rom);
        u8 *buf = (u8*)malloc(size);
        if (buf)
        {
            FileRead(&rom, buf, size);
            DrawPrintText("ROM loaded\n");
            free(buf);
        }
        else
        {
            DrawPrintText("No mem\n");
        }
        FileClose(&rom);
    }
    else
    {
        DrawPrintText("sample.min missing\n");
    }

    while (1) { }
    return 0;
}
