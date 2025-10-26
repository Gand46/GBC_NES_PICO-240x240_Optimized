// === DOOM application interface =============================================
// Lightweight façade that will host the rp2040-doom engine in task 2.  For now
// it provides a compilable stub with minimal rendering and input plumbing.

#pragma once

#include <cstdint>

class DoomApp {
public:
        void Init();
        void Shutdown();
        void Frame();
        bool ShouldExit() const;

private:
        bool requestExit = false;
        bool storageMounted = false;
        bool wadAvailable = false;

        void PollInput();
        void DrawPlaceholder();
};
