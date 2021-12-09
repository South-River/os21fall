#include "types.h"

struct run {
    struct run *next;
};

void mm_init();

unsigned long kalloc();
void kfree(unsigned long);