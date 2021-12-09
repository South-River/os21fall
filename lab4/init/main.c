#include "printk.h"
#include "print.h"
#include "sbi.h"
#include "clock.h"
#include "proc.h"

extern void test();

int start_kernel() {
    printk("Hello RISC-V\n");
    printk("idle process is running\n\n");
    test(); // DO NOT DELETE !!!
	return 0;
}
