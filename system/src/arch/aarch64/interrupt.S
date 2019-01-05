.globl asm_aarch64_enableInterrupts
asm_aarch64_enableInterrupts:
    msr daifclr, #2
    ret

.globl asm_aarch64_disableInterrupts
asm_aarch64_disableInterrupts:
    msr daifset, #2
    ret
