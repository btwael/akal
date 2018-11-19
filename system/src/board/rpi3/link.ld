MEMORY {
    RAM (xrw) : ORIGIN = 0x80000 , LENGTH = 0x20000000
}
SECTIONS {
    . = 0x80000;
    .text : {
        KEEP(*(.text.boot))
        *(.text .text.* .gnu.linkonce.t*)
    }
    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r*)
    }
    .data : {
        *(.data .data.* .gnu.linkonce.d*)
    }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end__ = .;
    }
}
