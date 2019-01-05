MEMORY {
    RAM (xrw) : ORIGIN = 0x80000 , LENGTH = 0x3B9ACA0
}
SECTIONS {
    . = 0x80000;
    __ld_start__ = .;
    .text : {
        . = ALIGN(4);
        _start = .;
        KEEP(*(.text.boot))
        *(.text .text.* .gnu.linkonce.t*)
        *(.textt)
    } > RAM
    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r*)
    } > RAM
    .init_array : {
        __init_start__ = .;
        *(.init_array*)
        __init_end__ = .;
    } > RAM
    .data : {
        *(.data .data.* .gnu.linkonce.d*)
    } > RAM
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM
    .stack_core0 : {
        . = ALIGN(16);
        __stack_cores0_start__ = .;
        . = . + 512;
        __stack_cores0_el0__ = .;
        . = . + 32768;
        __stack_cores0_el1__ = .;
        . = . + 16384;
        __stack_cores0_el2__ = .;
        . = ALIGN(16);
        __stack_cores0_end__ = .;  
    } > RAM
    .stack_core1 : {
        . = ALIGN(16);
        __stack_cores1_start__ = .;
        . = . + 512;
        __stack_cores1_el0__ = .;
        . = . + 32768;
        __stack_cores1_el1__ = .;
        . = . + 16384;
        __stack_cores1_el2__ = .;
        . = ALIGN(16);
        __stack_cores1_end__ = .;  
    } > RAM
    .stack_core2 : {
        . = ALIGN(16);
        __stack_cores2_start__ = .;
        . = . + 512;
        __stack_cores2_el0__ = .;
        . = . + 32768;
        __stack_cores2_el1__ = .;
        . = . + 16384;
        __stack_cores2_el2__ = .;
        . = ALIGN(16);
        __stack_cores2_end__ = .;  
    } > RAM
    .stack_core3 : {
        . = ALIGN(16);
        __stack_cores3_start__ = .;
        . = . + 512;
        __stack_cores3_el0__ = .;
        . = . + 32768;
        __stack_cores3_el1__ = .;
        . = . + 16384;
        __stack_cores3_el2__ = .;
        . = ALIGN(16);
        __stack_cores3_end__ = .;  
    } > RAM
    __ld_end__ = .;
}
