.section .bootloader, "a"
.global loader_entry
loader_entry:
    .space 32768
