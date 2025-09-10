/*
 * Minimal PicoPad bootloader placeholder.
 *
 * The RP2040 application linker script expects a 32 KB second stage
 * bootloader to be present in the `.boot2` section.  Until a real
 * bootloader implementation is provided, reserve the required space so
 * the final binary links correctly.  The section is marked executable so
 * it is kept even when the linker performs garbage collection.
 */

    .section .boot2, "ax"
    .global loader_entry
loader_entry:
    .space 32768
