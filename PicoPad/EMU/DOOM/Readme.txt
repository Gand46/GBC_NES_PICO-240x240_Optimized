DOOM (PicoPad) - Infraestructura de archivos
============================================

Este directorio define la base necesaria para portar rp2040-doom al ecosistema
de PicoPad siguiendo la misma organización aplicada a los emuladores de GBC y
NES. Además del esqueleto inicial, la tarea 5 incorpora la gestión de
almacenamiento y rutas que usará el motor definitivo.

Contenido destacado
-------------------
* `Makefile`: referencia todas las fuentes del motor importado y los módulos de
  plataforma, incluyendo `src/doom_files.cpp` para la preparación de rutas.
* `config.h`, `setup.h` e `include.h`: archivos de configuración listos para
  conectar con el SDK y con los periféricos del PicoPad.
* `src/`: contiene el bucle principal `main.cpp`, la clase `DoomApp` y los
  módulos auxiliares de audio, vídeo, entrada y archivos.
* Scripts `_c.bat`, `c.sh`, `d.*`, `e.*`: plantillas de compilación y despliegue
  equivalentes a las utilizadas en GBC y NES, pero apuntando al nuevo objetivo
  `DOOM`.

Preparación de archivos
-----------------------
1. Copia `doom1.wad` (o cualquier IWAD compatible como `doom.wad`, `doom2.wad`
   o `freedoom1.wad`) en la carpeta `/DOOM` de la tarjeta SD del PicoPad.
2. Si la aplicación se instala desde el cargador, la ruta real puede variar;
   al iniciar el port se mostrará en pantalla la ruta detectada para colocar
   los WAD.
3. Las partidas se almacenarán en `/DOOM/SAVE` (se crea automáticamente si no
   existe).

Al ejecutar la aplicación sin WAD, `DoomApp` informa en pantalla qué carpeta se
ha preparado y dónde deben copiarse los archivos necesarios. Cuando detecta un
WAD válido, confirma la ruta y deja la infraestructura lista para integrar el
motor en las tareas siguientes.

Próximos pasos
--------------
* Sustituir las implementaciones de marcador de posición por el código real del
  motor `rp2040-doom`.
* Mapear las entradas y salidas a los periféricos del PicoPad (teclas, pantalla
  ST7789 y audio PWM).
* Reutilizar esta estructura para incorporar el menú de ROMs, assets y
  configuraciones específicas del port.
