#include "../include.h"

void DoomApp::Init()
{
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
        u8 key = KeyGet();
        if ((key == KEY_Y) || (key == KEY_X))
        {
                requestExit = true;
                return;
        }

#if USE_USB_HOST_HID
        if (UsbKeyIsPressed(HID_KEY_ESCAPE) || UsbKeyIsPressed(HID_KEY_X))
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
        DrawText2("DOOM (stub)", (WIDTH - 11*16)/2, 32, COL_ORANGE);
        DrawText("El port se inicializar\303\241 en la siguiente tarea.", 12, 96, COL_WHITE);
        DrawText("Presiona Y para volver al cargador.", 12, 114, COL_GRAY);
        DrawText("Presiona X o ESC para simular salir del juego.", 12, 132, COL_GRAY);
        DispUpdateAll();
}
