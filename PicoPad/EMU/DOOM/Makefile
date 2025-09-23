# ASM source files
ASRC +=

# C source files
DOOM_ENGINE_DIR := src/engine
DOOM_CORE_DIR := ${DOOM_ENGINE_DIR}/doom
DOOM_CHOCOLATE_DIR := ${DOOM_ENGINE_DIR}/chocolate
DOOM_PORT_DIR := ${DOOM_ENGINE_DIR}/port

DOOM_CHOCOLATE_EXCLUDE := \
       ${DOOM_CHOCOLATE_DIR}/i_cdmus.c \
       ${DOOM_CHOCOLATE_DIR}/i_input.c \
       ${DOOM_CHOCOLATE_DIR}/i_joystick.c \
       ${DOOM_CHOCOLATE_DIR}/i_musicpack.c \
       ${DOOM_CHOCOLATE_DIR}/i_oplmusic.c \
       ${DOOM_CHOCOLATE_DIR}/i_pcsound.c \
       ${DOOM_CHOCOLATE_DIR}/i_sdlsound.c \
       ${DOOM_CHOCOLATE_DIR}/i_sdlmusic.c \
       ${DOOM_CHOCOLATE_DIR}/i_system.c \
       ${DOOM_CHOCOLATE_DIR}/i_timer.c \
       ${DOOM_CHOCOLATE_DIR}/i_video.c \
       ${DOOM_CHOCOLATE_DIR}/i_videohr.c \
       ${DOOM_CHOCOLATE_DIR}/net_client.c \
       ${DOOM_CHOCOLATE_DIR}/net_common.c \
       ${DOOM_CHOCOLATE_DIR}/net_dedicated.c \
       ${DOOM_CHOCOLATE_DIR}/net_gui.c \
       ${DOOM_CHOCOLATE_DIR}/net_io.c \
       ${DOOM_CHOCOLATE_DIR}/net_loop.c \
       ${DOOM_CHOCOLATE_DIR}/net_packet.c \
       ${DOOM_CHOCOLATE_DIR}/net_query.c \
       ${DOOM_CHOCOLATE_DIR}/net_server.c \
       ${DOOM_CHOCOLATE_DIR}/net_sdl.c \
       ${DOOM_CHOCOLATE_DIR}/net_structrw.c \
       ${DOOM_CHOCOLATE_DIR}/w_file_posix.c \
       ${DOOM_CHOCOLATE_DIR}/w_file_win32.c

CSRC += $(wildcard ${DOOM_CORE_DIR}/*.c)
CSRC += $(filter-out ${DOOM_CHOCOLATE_EXCLUDE},$(wildcard ${DOOM_CHOCOLATE_DIR}/*.c))
CSRC += ${DOOM_PORT_DIR}/compat.c
CSRC += ${DOOM_PORT_DIR}/doom_port_stubs.c

# C++ source files
SRC += ${DOOM_CHOCOLATE_DIR}/pd_render.cpp
SRC += src/doom_app.cpp
SRC += src/main.cpp

# Path to root directory from the project directory (without trailing '/' delimiter)
PICO_ROOT_PATH = ../../..

# Makefile includes
include ${PICO_ROOT_PATH}/Makefile.inc
