//trap.c
#include "clock.h"
#include "print.h"
#include "proc.h"

void trap_handler(unsigned long scause, unsigned long sepc)
{
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略

    //判断是否是interrupt
    
    if((int)scause >= 4)
    {
        //判断是否是timer interrupt
        if((int)scause == (int)5)
        {
            do_timer();
            clock_set_next_event();
        }
    }

    //puts("KERNEL RUNNING!\n");
    //puts("scause:");
    //puti(scause);
    //puts("\n");
    return;
}