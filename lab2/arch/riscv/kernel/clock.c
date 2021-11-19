//clock.c
#include "sbi.h"
#include "print.h"

// QEMU中时钟的频率是10MHz, 也就是1秒钟相当于10000000个时钟周期。
unsigned long TIMECLOCK = 10000000;

unsigned long get_cycles()
{
    // 使用 rdtime 编写内联汇编，获取 time 寄存器中 (也就是mtime 寄存器 )的值并返回
    unsigned long time = 0;

    asm volatile (
        "rdtime t0\n"
        "mv %[time],t0\n"
        :[time] "=r" (time)
        :
        :"memory"
    );
    return time;
}

void clock_set_next_event() {
    // 下一次 时钟中断 的时间点
    unsigned long next = get_cycles() + TIMECLOCK;
    puts("[S]Timer Interrupt\nNEXT INTERRUPT TIME:");
    puti(next/10000000);
    puts("\n");
    // 使用 sbi_ecall 来完成对下一次时钟中断的设置
    sbi_ecall(0x00, 0, next, 0, 0, 0, 0, 0);
    
    return;
} 

void first_timer_interrupt()
{
    unsigned long next = get_cycles();
    puts("[S]Timer Interrupt\nNEXT INTERRUPT TIME:");
    puti(next/10000000);
    puts("\n");
    // 使用 sbi_ecall 来完成对下一次时钟中断的设置
    sbi_ecall(0x00, 0, next, 0, 0, 0, 0, 0);
    
    return;
}
