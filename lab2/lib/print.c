#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    char* p=s;
    while(*p!='\0')
    {
	sbi_ecall(0x01,0x0,(int)(*p),0,0,0,0,0);
	p++;
    }
    return;
}

void puti(int x) {
    // unimplemented
    int temp=x;
    if(temp<0)
    {
    	sbi_ecall(0x01,0x00,45,0,0,0,0,0);
    	temp*=-1;
    }
    if(temp>9)
    {
    	puti(temp/10);
	sbi_ecall(0x01,0x00,0x30+temp%10,0,0,0,0,0);
    }    
    
    if(temp<=9)
    {
	sbi_ecall(0x01,0x00,0x30+temp,0,0,0,0,0);
        return;
    }
    return;
}
