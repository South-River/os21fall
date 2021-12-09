#include "defs.h"

.extern mm_init
.extern sbi_set_timer
.extern task_init
.extern early_pgtbl
.extern setup_vm
.extern setup_vm_final
.extern start_kernel

    .section .text.init
    .globl _start
    
_start:
    la sp,boot_stack_top

    jal setup_vm
    jal relocate
    jal mm_init
    jal setup_vm_final

   # set stvec = _traps
    la t0, _traps
    csrw stvec, t0

    # set sie[STIE] = 1
    li t0, 0x20
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
    jal task_init      
    jal start_kernel

relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
    #0xffffffe000000000 - 0x80000000 = 0xffffffdf80000000
    li t0, PA2VA_OFFSET
    add ra, ra, t0
    add sp, sp, t0
    
    ###################### 
    #   YOUR CODE HERE   #
    ######################

    # set satp with early_pgtbl
    #satp = 0b(Mode field)1000(ASID field)0000
    li t0, 0b1000
    slli t0, t0, 60
    la t1, early_pgtbl
    srli t1, t1, 12
    add t0, t0, t1
    csrw satp, t0

    ###################### 
    #   YOUR CODE HERE   #
    ######################

    # flush tlb
    sfence.vma zero, zero

    ret

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096  # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top:
