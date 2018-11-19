.section ".text.boot", "ax"

.globl _start
_start:
    bl      akal_main
hang:
    b       hang
