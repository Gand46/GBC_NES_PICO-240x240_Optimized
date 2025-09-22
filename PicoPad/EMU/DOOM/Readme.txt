DOOM (PicoPad) - Esqueleto del proyecto
=======================================

Este directorio define la estructura inicial necesaria para portar rp2040-doom
al ecosistema de PicoPad siguiendo la misma organizaci\303\263n aplicada a los
emuladores de GBC y NES.

Contenido destacado
-------------------
* `Makefile`: referencia \303\272nica a las nuevas fuentes `src/doom_app.cpp` y
  `src/main.cpp`.
* `config.h`, `setup.h` e `include.h`: archivos de configuraci\303\263n preparados
  para conectar con el SDK y con los perif\303\251ricos del PicoPad.
* `src/`: contiene un bucle principal y una clase `DoomApp` de marcador de
  posici\303\263n listos para integrar el motor de DOOM en la tarea siguiente.
* Scripts `_c.bat`, `c.sh`, `d.*`, `e.*`: plantillas de compilaci\303\263n y
  despliegue equivalentes a las utilizadas en GBC y NES, pero apuntando al
  nuevo objetivo `DOOM`.

Pr\303\263ximos pasos
----------------
* Sustituir las implementaciones de marcador de posici\303\263n por el c\303\263digo real
  del motor `rp2040-doom`.
* Mapear las entradas y salidas a los perif\303\251ricos del PicoPad (teclas,
  pantalla ST7789 y audio PWM).
* Reutilizar esta estructura para incorporar el men\303\272 de ROMs, assets y
  configuraciones espec\303\255ficas del port.
