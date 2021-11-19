    .section .text.entry
    .align 2
    .globl _traps 
    #.globl trap_handler
_traps:

    # -----------
    # 1.save 32 registers and sepc to stack
    
    addi sp, sp, -0x100

    # save 32 registers
    # sd x0,  0x00(sp)
    sd ra, 0x08(sp)
    # sd x2 0x10(sp)
    sd gp, 0x18(sp)
    sd tp, 0x20(sp)
    sd t0, 0x28(sp)
    sd t1, 0x30(sp)
    sd t2, 0x38(sp)
    sd s0, 0x40(sp)
    sd s1, 0x48(sp)
    sd a0, 0x50(sp)
    sd a1, 0x58(sp)
    sd a2, 0x60(sp)
    sd a3, 0x68(sp)
    sd a4, 0x70(sp)
    sd a5, 0x78(sp)
    sd a6, 0x80(sp)
    sd a7, 0x88(sp)
    sd s2, 0x90(sp)
    sd s3, 0x98(sp)
    sd s4, 0xA0(sp)
    sd s5, 0xA8(sp)
    sd s6, 0xB0(sp)
    sd s7, 0xB8(sp)
    sd s8, 0xC0(sp)
    sd s9, 0xC8(sp)
    sd s10, 0xD0(sp)
    sd s11, 0xD8(sp)
    sd t3, 0xE0(sp)
    sd t4, 0xE8(sp)
    sd t5, 0xF0(sp)
    sd t6, 0xF8(sp)

    sd x2,  0x10(sp)

    # save sepc
    csrr t0, sepc
    sd t0, 0x00(sp)
    
    # -----------
    #2. call trap_handler
    csrr t0, scause
    csrr t1, sepc
    mv a0,t0
    mv a1,t1
    jal trap_handler

    # -----------
    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    
    #restore sepc
    ld t0, 0x00(sp)
    csrw sepc, t0

    # restore32 registers
    # ld x0  0x00(sp)
    ld ra, 0x08(sp)
    # ld x2 0x10(sp)
    ld gp, 0x18(sp)
    ld tp, 0x20(sp)
    ld t0, 0x28(sp)
    ld t1, 0x30(sp)
    ld t2, 0x38(sp)
    ld s0, 0x40(sp)
    ld s1, 0x48(sp)
    ld a0, 0x50(sp)
    ld a1, 0x58(sp)
    ld a2, 0x60(sp)
    ld a3, 0x68(sp)
    ld a4, 0x70(sp)
    ld a5, 0x78(sp)
    ld a6, 0x80(sp)
    ld a7, 0x88(sp)
    ld s2, 0x90(sp)
    ld s3, 0x98(sp)
    ld s4, 0xA0(sp)
    ld s5, 0xA8(sp)
    ld s6, 0xB0(sp)
    ld s7, 0xB8(sp)
    ld s8, 0xC0(sp)
    ld s9, 0xC8(sp)
    ld s10, 0xD0(sp)
    ld s11, 0xD8(sp)
    ld t3, 0xE0(sp)
    ld t4, 0xE8(sp)
    ld t5, 0xF0(sp)
    ld t6, 0xF8(sp)

    ld x2, 0x10(sp)

    addi sp,sp,0x100
    # -----------
    # 4. return from trap
    sret