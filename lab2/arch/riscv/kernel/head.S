.extern start_kernel

    .section .text.init
    .globl _start
    
_start:

    # set stvec = _traps
    la t0,_traps
    csrw stvec,t0

    # set sie[STIE] = 1
    li t0,0x20
    csrw sie,t0

    #set first timer interrupt
    #clock_set_next_event()
    #addi sp,sp,-16
    #sd ra,8(sp)
    #rdtime t0
    #mv a1,t0
    #auipc a2,0x2
    #ld a2,-396(a2)
    #add a2,a1,a2
    #li a7,0
    #li a6,0
    #li a5,0
    #li a4,0
    #li a3,0
    #li a1,0
    #li a0,0
    #jal ra,0x802001cc
    #ld ra,8(sp)
    #auipc a0,0x1
    #addi a0,a0,-444
    #addi sp,sp,16
    #j 0x80200248


    # set sstatus[SIE] = 1
    li t0,0x02
    csrw sstatus,t0

    # ------------------
    # - your code here -
    # ------------------
    la sp,boot_stack_top

    jal start_kernel
.section .bss.stack


    .globl boot_stack
boot_stack:
    .space 4096  # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top:
