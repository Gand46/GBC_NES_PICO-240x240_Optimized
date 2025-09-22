// === DOOM emulator setup =====================================================
// These constants describe the runtime configuration expected by the
// forthcoming rp2040-doom port. They are intentionally lightweight so the
// skeleton can compile while the real engine is integrated in task 2.

#pragma once

// Desired tick rate of the original DOOM engine (35 Hz).
#define DOOM_TICK_RATE          35

// Nominal internal render resolution produced by the software renderer.
#define DOOM_INTERNAL_WIDTH     320
#define DOOM_INTERNAL_HEIGHT    200

// Scaling behaviour from the DOOM buffer to the PicoPad LCD.
#define DOOM_DISPLAY_WIDTH      240
#define DOOM_DISPLAY_HEIGHT     240

// Path configuration helpers.
#define DOOM_APP_FOLDER         "/DOOM"
#define DOOM_SAVE_FOLDER        "/DOOM/SAVE"

// Simple audio defaults (will be refined when the real mixer is wired).
#define DOOM_AUDIO_SAMPLE_RATE  22050
#define DOOM_AUDIO_BUFFER       512
