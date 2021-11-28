.extern start_kernel

    .section .text.init
    .globl _start
    
_start:
    la sp,boot_stack_top
   
   # set stvec = _traps
    la t0, _traps
    csrw stvec, t0

    # set sie[STIE] = 1
    li t0,0x20
    csrw sie,t0
    
    #set first timer interrupt
    rdtime a0
    li t0, 10000000
    add a0, a0, t0
    li a6, 0
    li a7, 0
    ecall

    # set sstatus[SIE] = 1
    li t0,0x02
    csrw sstatus,t0    
 
    #call task_init in proper position in _start
    jal mm_init
    jal task_init   
    jal start_kernel
.section .bss.stack

    .globl boot_stack
boot_stack:
    .space 4096  # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top:
