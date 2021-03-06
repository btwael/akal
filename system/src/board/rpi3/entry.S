.section ".textt"

.globl irq_vector_init
irq_vector_init:
    adr x0, vectors         // load VBAR_EL1 with virtual
    msr vbar_el1, x0        // vector table address
    ret

.globl generic_timer_init
generic_timer_init:
    mov x0, #1
    msr CNTP_CTL_EL0, x0
    ret

.globl generic_timer_reset
generic_timer_reset:
    mov x0, #1
    lsl x0, x0, #24 
    msr CNTP_TVAL_EL0, x0
    ret

.globl ret_from_fork
ret_from_fork:
    bl schedule_tail
    mov    x0, x20
    blr    x19
    mov x0, x21
    bl end_pid
    bl schedule

.globl cpu_switch_to
cpu_switch_to:
    mov    x10, #0
    add    x8, x0, x10
    mov    x9, sp
    stp    x19, x20, [x8], #16        // store callee-saved registers
    stp    x21, x22, [x8], #16
    stp    x23, x24, [x8], #16
    stp    x25, x26, [x8], #16
    stp    x27, x28, [x8], #16
    stp    x29, x9, [x8], #16
    str    x30, [x8]
    add    x8, x1, x10
    ldp    x19, x20, [x8], #16        // restore callee-saved registers
    ldp    x21, x22, [x8], #16
    ldp    x23, x24, [x8], #16
    ldp    x25, x26, [x8], #16
    ldp    x27, x28, [x8], #16
    ldp    x29, x9, [x8], #16
    ldr    x30, [x8]
    mov    sp, x9
    ret

#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)
#define SCTLR_I_CACHE_DISABLED          (0 << 12)
#define SCTLR_D_CACHE_DISABLED          (0 << 2)
#define SCTLR_MMU_DISABLED              (0 << 0)
#define SCTLR_MMU_ENABLED               (1 << 0)

#define SCTLR_VALUE_MMU_DISABLED    (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

#define HCR_RW                  (1 << 31)
#define HCR_VALUE           HCR_RW

#define SPSR_MASK_ALL           (7 << 6)
#define SPSR_EL1h           (5 << 0)
#define SPSR_VALUE          (SPSR_MASK_ALL | SPSR_EL1h)

#define SCR_RESERVED                (3 << 4)
#define SCR_RW              (1 << 10)
#define SCR_NS              (1 << 0)
#define SCR_VALUE                   (SCR_RESERVED | SCR_RW | SCR_NS)

.globl asm_rpi3_aarch64_switchToEL1
asm_rpi3_aarch64_switchToEL1:
#ifndef AKAL_APPLICATION_TARGET_RPI3QEMU
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0       

    ldr x0, =HCR_VALUE
    msr hcr_el2, x0

    ldr x0, =SCR_VALUE
    msr scr_el3, x0

    ldr x0, =SPSR_VALUE
    msr spsr_el3, x0

    adr x0, el1_entry       
    msr elr_el3, x0
#else
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0       

    ldr x0, =HCR_VALUE
    msr hcr_el2, x0
    
    ldr x0, =SPSR_VALUE
    msr spsr_el2, x0        // 'spsr_el3' changed to 'spsr_el2'

    adr x0, el1_entry       
    msr elr_el2, x0         // 'elr_el3' changed to 'elr_el2'
#endif
    eret
el1_entry: ret


#define S_FRAME_SIZE            256         // size of all saved registers 

#define SYNC_INVALID_EL1t       0 
#define IRQ_INVALID_EL1t        1 
#define FIQ_INVALID_EL1t        2 
#define ERROR_INVALID_EL1t      3 

#define SYNC_INVALID_EL1h       4 
#define IRQ_INVALID_EL1h        5 
#define FIQ_INVALID_EL1h        6 
#define ERROR_INVALID_EL1h      7 

#define SYNC_INVALID_EL0_64         8 
#define IRQ_INVALID_EL0_64          9 
#define FIQ_INVALID_EL0_64      10 
#define ERROR_INVALID_EL0_64        11 

#define SYNC_INVALID_EL0_32     12 
#define IRQ_INVALID_EL0_32      13 
#define FIQ_INVALID_EL0_32      14 
#define ERROR_INVALID_EL0_32        15 

    .macro handle_invalid_entry type
    kernel_entry
    mov x0, #\type
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl  show_invalid_entry_message
    b   err_hang
    .endm

    .macro  ventry  label
    .align  7
    b   \label
    .endm

    .macro  kernel_entry
    sub sp, sp, #S_FRAME_SIZE
    stp x0, x1, [sp, #16 * 0]
    stp x2, x3, [sp, #16 * 1]
    stp x4, x5, [sp, #16 * 2]
    stp x6, x7, [sp, #16 * 3]
    stp x8, x9, [sp, #16 * 4]
    stp x10, x11, [sp, #16 * 5]
    stp x12, x13, [sp, #16 * 6]
    stp x14, x15, [sp, #16 * 7]
    stp x16, x17, [sp, #16 * 8]
    stp x18, x19, [sp, #16 * 9]
    stp x20, x21, [sp, #16 * 10]
    stp x22, x23, [sp, #16 * 11]
    stp x24, x25, [sp, #16 * 12]
    stp x26, x27, [sp, #16 * 13]
    stp x28, x29, [sp, #16 * 14]

    mrs x22, elr_el1
    mrs x23, spsr_el1

    stp x30, x22, [sp, #16 * 15] 
    str x23, [sp, #16 * 16]
    .endm

    .macro  kernel_exit
    ldr x23, [sp, #16 * 16]
    ldp x30, x22, [sp, #16 * 15] 

    msr elr_el1, x22            
    msr spsr_el1, x23

    ldp x0, x1, [sp, #16 * 0]
    ldp x2, x3, [sp, #16 * 1]
    ldp x4, x5, [sp, #16 * 2]
    ldp x6, x7, [sp, #16 * 3]
    ldp x8, x9, [sp, #16 * 4]
    ldp x10, x11, [sp, #16 * 5]
    ldp x12, x13, [sp, #16 * 6]
    ldp x14, x15, [sp, #16 * 7]
    ldp x16, x17, [sp, #16 * 8]
    ldp x18, x19, [sp, #16 * 9]
    ldp x20, x21, [sp, #16 * 10]
    ldp x22, x23, [sp, #16 * 11]
    ldp x24, x25, [sp, #16 * 12]
    ldp x26, x27, [sp, #16 * 13]
    ldp x28, x29, [sp, #16 * 14]
    add sp, sp, #S_FRAME_SIZE       
    eret
    .endm


/*
 * Exception vectors.
 */
.align  11
.globl vectors 
vectors:
    ventry  sync_invalid_el1t           // Synchronous EL1t
    ventry  el1_irq            // IRQ EL1t
    ventry  fiq_invalid_el1t            // FIQ EL1t
    ventry  error_invalid_el1t          // Error EL1t

    ventry  sync_invalid_el1h           // Synchronous EL1h
    ventry  el1_irq                 // IRQ EL1h
    ventry  fiq_invalid_el1h            // FIQ EL1h
    ventry  error_invalid_el1h          // Error EL1h

    ventry  sync_invalid_el0_64         // Synchronous 64-bit EL0
    ventry  irq_invalid_el0_64          // IRQ 64-bit EL0
    ventry  fiq_invalid_el0_64          // FIQ 64-bit EL0
    ventry  error_invalid_el0_64            // Error 64-bit EL0

    ventry  sync_invalid_el0_32         // Synchronous 32-bit EL0
    ventry  irq_invalid_el0_32          // IRQ 32-bit EL0
    ventry  fiq_invalid_el0_32          // FIQ 32-bit EL0
    ventry  error_invalid_el0_32            // Error 32-bit EL0

sync_invalid_el1t:
    handle_invalid_entry  SYNC_INVALID_EL1t

irq_invalid_el1t:
    handle_invalid_entry  IRQ_INVALID_EL1t

fiq_invalid_el1t:
    handle_invalid_entry  FIQ_INVALID_EL1t

error_invalid_el1t:
    handle_invalid_entry  ERROR_INVALID_EL1t

sync_invalid_el1h:
    handle_invalid_entry  SYNC_INVALID_EL1h

fiq_invalid_el1h:
    handle_invalid_entry  FIQ_INVALID_EL1h

error_invalid_el1h:
    handle_invalid_entry  ERROR_INVALID_EL1h

sync_invalid_el0_64:
    handle_invalid_entry  SYNC_INVALID_EL0_64

irq_invalid_el0_64:
    handle_invalid_entry  IRQ_INVALID_EL0_64

fiq_invalid_el0_64:
    handle_invalid_entry  FIQ_INVALID_EL0_64

error_invalid_el0_64:
    handle_invalid_entry  ERROR_INVALID_EL0_64

sync_invalid_el0_32:
    handle_invalid_entry  SYNC_INVALID_EL0_32

irq_invalid_el0_32:
    handle_invalid_entry  IRQ_INVALID_EL0_32

fiq_invalid_el0_32:
    handle_invalid_entry  FIQ_INVALID_EL0_32

error_invalid_el0_32:
    handle_invalid_entry  ERROR_INVALID_EL0_32

el1_irq:
    kernel_entry 
    bl  handle_irq
    kernel_exit 

.globl err_hang
err_hang: b err_hang
