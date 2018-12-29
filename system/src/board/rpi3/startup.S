.section ".text.boot", "ax"

.globl _start
_start:
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 3f
    // cpu id > 0, stop
2:  wfe
    b       2b
3:
    bl coreStackInit

/*    // switch EL1
    mov     x0, #0x3c5
    msr     spsr_el2, x0
    adr     x0, 1f
    msr     elr_el2, x0
    eret
1:*/
    bl      akal_main
hang:
    b       2b

coreStackInit:
    ldr     x2, =__stack_cores0_el2__
    ldr     x3, =__stack_cores0_el1__
    ldr     x4, =__stack_cores0_el0__
    mrs     x0, mpidr_el1
    ands    x0, x0, #0x3
    beq     .set_stacks
    /*ldr     x2, =__stack_cores1_el2__
    ldr     x3, =__stack_cores1_el1__
    ldr     x4, =__stack_cores1_el0__
    cmp     x0, #1
    beq     .set_stacks
    ldr     x2, =__stack_cores2_el2__
    ldr     x3, =__stack_cores2_el1__
    ldr     x4, =__stack_cores2_el0__
    cmp     x0, #2
    beq     .set_stacks
    ldr     x2, =__stack_cores2_el2__
    ldr     x3, =__stack_cores2_el1__
    ldr     x4, =__stack_cores2_el0__*/
.set_stacks:
    mov     sp, x2
    msr     sp_el1, x3
    msr     sp_el0, x4
    ret
