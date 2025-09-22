// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "../include.h"

int main()
{
#if USE_USB_HOST_HID
        UsbHostInit();
#endif

        DoomApp app;
        app.Init();

        while (!app.ShouldExit())
        {
                app.Frame();
        }

        app.Shutdown();
        ResetToBootLoader();
        return 0;
}
