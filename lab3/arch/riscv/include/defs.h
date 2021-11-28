#ifndef _DEFS_H
#define _DEFS_H

#include "types.h"

//lab3 add
#define PHY_START 0x0000000080000000
#define PHY_SIZE  128 * 1024 * 1024 // 128MB， QEMU 默认内存大小
#define PHY_END   (PHY_START + PHY_SIZE)

#define PGSIZE 0x1000 // 4KB
#define PGROUNDUP(addr) ((addr + PGSIZE - 1) & (~(PGSIZE - 1)))
#define PGROUNDDOWN(addr) (addr & (~(PGSIZE - 1)))
//end

#define csr_read(csr)                       		\
({                                          		\
    register uint64 __v;                    		\
    /* unimplemented */                     		\
    asm volatile ("csrr %[val] " #csr "\n"          \
                    : [val] "=r" (__v) 			    \
  		            :   		                    \
                    : "memory"); 			        \
    __v;                                    		\
})

#define csr_write(csr, val)                         	\
({                                                  	\
    uint64 __v = (uint64)(val);                     	\
    asm volatile ("csrw " #csr ", %0"               	\
                    : : "r" (__v)                   	\
                    : "memory");                    	\
})

#endif
