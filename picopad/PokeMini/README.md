# PokeMini Emulator Port (Work in Progress)

This directory contains an initial skeleton for a port of the
[PokeMini](https://github.com/libretro/PokeMini) emulator to the PicoPad SDK.

## Directory Structure
- `src/` – source code of the emulator (currently a minimal RP2040 test stub).
- `samples/` – place `.min` ROM files here to be built into the UF2 image.
- `_c.bat` – Windows batch file to build the project using the existing SDK scripts.
- `d.bat` – clean script removing build artifacts.

## Status
Only a minimal LED blink test is implemented to validate the project setup.
Full integration of the PokeMini emulator sources is still pending.
