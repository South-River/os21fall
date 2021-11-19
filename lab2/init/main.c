#include "printk.h"
#include "print.h"
#include "sbi.h"
#include "clock.h"
#include "trap.h"

extern void test();

int start_kernel() {
    //first_timer_interrupt();
    clock_set_next_event();
    puts("KERNEL IS RUNNING!\n\n");
    test(); // DO NOT DELETE !!!
	return 0;
}
