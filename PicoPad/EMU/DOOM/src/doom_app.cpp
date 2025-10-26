#include "../include.h"

void DoomApp::Init()
{
        DoomInputInit();
        DoomFilesInit();

        const DoomFileSystemStatus& status = DoomFilesGetStatus();
        storageMounted = status.storageMounted;
        wadAvailable = status.wadFound;
        DrawPlaceholder();
}

void DoomApp::Shutdown()
{
        DrawClear();
        DispUpdateAll();
}

void DoomApp::Frame()
{
        PollInput();
        // Limit the loop to roughly the DOOM tick rate so the skeleton behaves.
        WaitMs(1000 / DOOM_TICK_RATE);
}

bool DoomApp::ShouldExit() const
{
        return requestExit;
}

void DoomApp::PollInput()
{
        if (DoomInputConsumeExitRequest())
        {
                requestExit = true;
                return;
        }

#if USE_USB_HOST_HID
        if (UsbKeyIsPressed(HID_KEY_ESCAPE))
        {
                requestExit = true;
                return;
        }
#endif
}

void DoomApp::DrawPlaceholder()
{
        // Prepare a neutral background until the real renderer is wired.
        DrawClear();
        SelFont8x16();
        DrawText2("DOOM (archivos)", (WIDTH - 15*16)/2, 32, COL_ORANGE);

        SelFont8x8();
        int y = 88;

        const DoomFileSystemStatus& status = DoomFilesGetStatus();

        if (!storageMounted)
        {
                DrawText("No se detecta almacenamiento FAT.", 12, y, COL_WHITE); y += 10;
                DrawText("Inserta una tarjeta SD con /DOOM.", 12, y, COL_GRAY); y += 10;
                DrawText("Reinicia el port tras montar la unidad.", 12, y, COL_GRAY);
        }
        else if (!wadAvailable)
        {
                DrawText("Carpeta de DOOM preparada en:", 12, y, COL_WHITE); y += 10;
                DrawText(status.basePath, 12, y, COL_YELLOW); y += 10;
                DrawText("Copia doom1.wad o un IWAD compatible", 12, y, COL_GRAY); y += 10;
                DrawText("dentro de esa carpeta y vuelve a iniciar.", 12, y, COL_GRAY); y += 10;
                DrawText("Las partidas se guardar\303\241n en:", 12, y, COL_WHITE); y += 10;
                DrawText(status.savePath, 12, y, COL_YELLOW);
        }
        else
        {
                DrawText("Sistema listo para integrar el motor.", 12, y, COL_WHITE); y += 10;
                DrawText("WAD detectado:", 12, y, COL_GRAY); y += 10;
                DrawText(status.wadPath, 12, y, COL_YELLOW); y += 10;
                DrawText("Las partidas usar\303\241n:", 12, y, COL_GRAY); y += 10;
                DrawText(status.savePath, 12, y, COL_YELLOW); y += 10;
                DrawText("Contin\303\272a con la tarea 2 para ejecutar DOOM.", 12, y, COL_WHITE);
        }

        SelFont8x16();
        DrawText("Presiona Y para volver al cargador.", 12, HEIGHT - 32, COL_GRAY);
        DrawText("Presiona X o ESC para simular salir.", 12, HEIGHT - 16, COL_GRAY);
        DispUpdateAll();
}
