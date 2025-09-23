#include "../include.h"

#include "doom_input.h"

extern "C" {
#include "engine/chocolate/d_event.h"
#include "engine/chocolate/doomkeys.h"
#include "engine/chocolate/i_input.h"
}

#include <array>
#include <cstddef>
#include <cstdint>

namespace
{

enum class BindingSource : uint8_t
{
        Physical,
        Usb,
};

struct BindingEntry
{
        BindingSource source;
        uint8_t code;
};

struct KeyBinding
{
        int doom_key;
        int ascii;
        std::array<BindingEntry, 8> bindings;
        std::size_t binding_count;
        bool pressed;
};

constexpr BindingEntry MakePhysical(uint8_t code)
{
        return BindingEntry{BindingSource::Physical, code};
}

constexpr BindingEntry MakeUsb(uint8_t code)
{
        return BindingEntry{BindingSource::Usb, code};
}

bool IsPressed(const BindingEntry &binding)
{
        if (binding.source == BindingSource::Physical)
        {
                return KeyPressed(binding.code);
        }

#if USE_USB_HOST_HID
        if (binding.source == BindingSource::Usb)
        {
                return UsbKeyIsPressed(binding.code);
        }
#else
        (void)binding;
#endif
        return false;
}

void PostKeyEvent(int doom_key, bool pressed, int ascii)
{
        event_t ev;
        ev.type = pressed ? ev_keydown : ev_keyup;
        ev.data1 = doom_key;
        if (pressed)
        {
                ev.data2 = ascii;
                ev.data3 = ascii;
        }
        else
        {
                ev.data2 = 0;
                ev.data3 = 0;
        }
        ev.data4 = 0;
        ev.data5 = 0;
        D_PostEvent(&ev);
}

void PostKeyTap(int doom_key, int ascii)
{
        PostKeyEvent(doom_key, true, ascii);
        PostKeyEvent(doom_key, false, ascii);
}

KeyBinding key_bindings[] = {
        { KEY_UPARROW, 0,
          { MakePhysical(KEY_UP)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_ARROW_UP), MakeUsb(HID_KEY_W)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_DOWNARROW, 0,
          { MakePhysical(KEY_DOWN)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_ARROW_DOWN), MakeUsb(HID_KEY_S)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_LEFTARROW, 0,
          { MakePhysical(KEY_LEFT)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_ARROW_LEFT), MakeUsb(HID_KEY_A)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_RIGHTARROW, 0,
          { MakePhysical(KEY_RIGHT)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_ARROW_RIGHT), MakeUsb(HID_KEY_D)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_RCTRL, 0,
          { MakePhysical(KEY_A)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_CONTROL_LEFT), MakeUsb(HID_KEY_CONTROL_RIGHT)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_SPACE, ' ',
          { MakePhysical(KEY_B)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_SPACE), MakeUsb(HID_KEY_KEYPAD_ENTER)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_RSHIFT, 0,
          { MakePhysical(KEY_X)
#if USE_USB_HOST_HID
          , MakeUsb(HID_KEY_SHIFT_LEFT), MakeUsb(HID_KEY_SHIFT_RIGHT)
#endif
          },
#if USE_USB_HOST_HID
          3,
#else
          1,
#endif
          false },

        { KEY_ESCAPE, KEY_ESCAPE,
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_ESCAPE)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { KEY_ENTER, KEY_ENTER,
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_ENTER), MakeUsb(HID_KEY_KEYPAD_ENTER)
#endif
          },
#if USE_USB_HOST_HID
          2,
#else
          0,
#endif
          false },

        { KEY_TAB, '\t',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_TAB)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '1', '1',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_1)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '2', '2',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_2)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '3', '3',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_3)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '4', '4',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_4)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '5', '5',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_5)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '6', '6',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_6)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '7', '7',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_7)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '8', '8',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_8)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '9', '9',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_9)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },

        { '0', '0',
          {
#if USE_USB_HOST_HID
            MakeUsb(HID_KEY_0)
#endif
          },
#if USE_USB_HOST_HID
          1,
#else
          0,
#endif
          false },
};

bool exit_requested = false;
bool menu_requested = false;
bool combo_latched = false;
bool prev_select = false;

bool GetStartPressed()
{
        bool pressed = KeyPressed(KEY_X);
#if USE_USB_HOST_HID
        pressed = pressed || UsbKeyIsPressed(HID_KEY_ENTER) || UsbKeyIsPressed(HID_KEY_KEYPAD_ENTER);
#endif
        return pressed;
}

bool GetSelectPressed()
{
        bool pressed = KeyPressed(KEY_Y);
#if USE_USB_HOST_HID
        pressed = pressed || UsbKeyIsPressed(HID_KEY_BACKSPACE);
#endif
        return pressed;
}

void HandleSpecialCombos(bool start_pressed, bool select_pressed)
{
        if (start_pressed && select_pressed)
        {
                if (!combo_latched)
                {
                        combo_latched = true;
                        PostKeyTap(KEY_ESCAPE, KEY_ESCAPE);
                        menu_requested = true;
                }
        }
        else
        {
                combo_latched = false;
                if (select_pressed && !prev_select && !start_pressed)
                {
                        exit_requested = true;
                }
        }

        prev_select = select_pressed;
}

} // namespace

extern "C" float mouse_acceleration = 0.0f;
extern "C" int mouse_threshold = 0;

extern "C" void DoomInputInit(void)
{
        for (KeyBinding &binding : key_bindings)
        {
                binding.pressed = false;
        }

        exit_requested = false;
        menu_requested = false;
        combo_latched = false;
        prev_select = GetSelectPressed();
}

extern "C" bool DoomInputConsumeExitRequest(void)
{
        const bool requested = exit_requested;
        exit_requested = false;
        return requested;
}

extern "C" bool DoomInputConsumeMenuRequest(void)
{
        const bool requested = menu_requested;
        menu_requested = false;
        return requested;
}

extern "C" void I_StartTic(void)
{
        for (KeyBinding &binding : key_bindings)
        {
                if (binding.binding_count == 0)
                {
                        continue;
                }

                bool pressed = false;
                for (std::size_t i = 0; i < binding.binding_count; ++i)
                {
                        if (IsPressed(binding.bindings[i]))
                        {
                                pressed = true;
                                break;
                        }
                }

                if (pressed != binding.pressed)
                {
                        PostKeyEvent(binding.doom_key, pressed, binding.ascii);
                        binding.pressed = pressed;
                }
        }

        HandleSpecialCombos(GetStartPressed(), GetSelectPressed());
}

extern "C" void I_BindInputVariables(void)
{
}

extern "C" void I_ReadMouse(void)
{
}

extern "C" void I_StartTextInput(int x1, int y1, int x2, int y2)
{
        (void)x1;
        (void)y1;
        (void)x2;
        (void)y2;
}

extern "C" void I_StopTextInput(void)
{
}

