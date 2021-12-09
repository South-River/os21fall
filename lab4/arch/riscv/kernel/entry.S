.extern trap_handler
.extern dummy

.altmacro
.set REG_SIZE, 8
.set CONTEXT_SIZE, 34

.macro SAVE reg1, reg2, offset
    sd \reg1, \offset*8(\reg2)
.endm

.macro SAVE_X_N reg, n
    SAVE x\n, \reg, \n
.endm

.macro SAVE_S_N reg, n
    SAVE s\n, \reg, \n
.endm

.macro LOAD reg1, reg2, offset
    ld \reg1, \offset*8(\reg2)
.endm

.macro LOAD_X_N reg, n
    LOAD x\n, \reg, \n
.endm

.macro LOAD_S_N reg, n
    LOAD s\n, \reg, \n
.endm

    .section .text.entry
    .align 2
    .globl _traps 
_traps:
    # save 32 registers and sepc to stack
    addi sp, sp, -33*8

    SAVE x1, sp, 1
    addi x1, sp, 33*8
    SAVE x1, sp, 2

    .set n, 3
    .rept 29
        SAVE_X_N sp, %n
        .set n, n + 1
    .endr

    csrr t0, sepc
    SAVE t0, sp, 32

    # call trap_handler
    csrr a0, scause
    csrr a1, sepc
    call trap_handler

    # restore sepc and 32 registers (x2(sp) should be restore last) from stack
    LOAD t0, sp, 32
    csrw sepc, t0

    LOAD x1, sp, 1
    .set n, 3
    .rept 29
        LOAD_X_N sp, %n
        .set n, n + 1
    .endr

    LOAD x2, sp, 2

    # return from trap
    sret

    .globl __switch_to
__switch_to:
    # save state to prev process
    addi t0, a0, 40
    sd ra, 0(t0)
    sd sp, 8(t0)

    addi t0, a0, 56
    .set n, 0
    .rept 12
        SAVE_S_N t0, %n
        .set n, n+1
    .endr

    # restore state from next process
    addi t0, a1, 40
    ld ra, 0(t0)
    ld sp, 8(t0)

    addi t0, a1, 56
    .set n, 0
    .rept 12
        LOAD_S_N t0, %n
        .set n, n+1
    .endr

    ret

    .globl __dummy
__dummy:
    la t0, dummy
    csrw sepc, t0
    sret