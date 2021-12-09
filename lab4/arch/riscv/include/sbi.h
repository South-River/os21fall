#ifndef _SBI_H
#define _SBI_H

#define SBI_PUTCHAR 0x1

#include "types.h"

struct sbiret {
	long error;
	long value;
};

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
			            unsigned long arg1, unsigned long arg2,
			            unsigned long arg3, unsigned long arg4,
			            unsigned long arg5);
 
#endif
