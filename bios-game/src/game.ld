ENTRY(game_start)

SECTIONS {
    . = 0x8000;
    .start : { *(.start) }
    .text : { *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    .bss : { *(.bss .bss.*) *(COMMON) }
    ASSERT(. <= 0xa000, "Game exceeds the 16 sectors loaded by boot.S")
    /DISCARD/ : { *(.comment .note* .eh_frame*) }
}
