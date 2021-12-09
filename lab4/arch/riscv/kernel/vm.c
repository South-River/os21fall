// arch/riscv/kernel/vm.c
#include "defs.h"
#include "mm.h"
#include <string.h>

/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void pre_mapping(unsigned long* pgtbl, unsigned long va, unsigned long pa, int perm)
{
    int idx = (va>>30) & 0x1ff;
    pgtbl[idx] = (perm & 0b1111) | ((pa >> 30) & 0x3ffff) << 28;
}

void setup_vm(void) 
{
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */
    memset(early_pgtbl, 0x0, PGSIZE);

    unsigned long pa = PHY_START;
    unsigned long va = PHY_START;
    pre_mapping(early_pgtbl, va, pa, 0b1111);

    va = VM_START;
    pre_mapping(early_pgtbl, va, pa, 0b1111);
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

/* 创建多级页表映射关系 */
void create_mapping(unsigned long *root_pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm) {
    /*
    root_pgtbl 为根页表的基地址
    va, pa 为需要映射的虚拟地址、物理地址
    sz 为映射的大小
    perm 为映射的读写权限

    创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
    可以使用 V bit 来判断页表项是否存在
    */
    unsigned int VPN[3];
    unsigned long* pgtbl[3];
    unsigned long pte[3];
    unsigned long* new_page;
    unsigned long va_end = va + sz;

    while(va < va_end)
    {
        pgtbl[2] = root_pgtbl;
        VPN[2] = (va>>30)&0x1ff;
        pte[2] = pgtbl[2][VPN[2]];
        if(!(pte[2]&1))
        {
            new_page = (unsigned long*) kalloc();
            pte[2] = ((((unsigned long)new_page-PA2VA_OFFSET) >> 12) << 10) | 1;
            pgtbl[2][VPN[2]] = pte[2];
        }

        pgtbl[1] = (unsigned long*)((pte[2]>>10)<<12);
        VPN[1] = (va>>21)&0x1ff;
        pte[1] = pgtbl[1][VPN[1]];
        if(!pte[1]&1)
        {
            new_page = (unsigned long*) kalloc();
            pte[1] = ((((unsigned long)new_page - PA2VA_OFFSET)>>12)<<10)|1;
            pgtbl[1][VPN[1]] = pte[1];
        }
        
        pgtbl[0] = (unsigned long*)((pte[1]>>10)<<12);
        VPN[0] = (va>>12)&0x1ff;
        pte[0] = (perm&0b1111)|((pa>>12)<<10);
        pgtbl[0][VPN[0]] = pte[0];
        
        va += PGSIZE;
        pa += PGSIZE;
    }
}

extern char _stext[];
extern char _srodata[];
extern char _sdata[];

void setup_vm_final(void) {
    memset(swapper_pg_dir, 0x0, PGSIZE);
    // No OpenSBI mapping required

    // mapping kernel text X|-|R|V    
    unsigned long va = VM_START + OPENSBI_SIZE;
    unsigned long pa = PHY_START + OPENSBI_SIZE;
    unsigned long sz = (unsigned long)_srodata - (unsigned long)_stext;
    create_mapping(swapper_pg_dir, va, pa, sz, 0b1011);

    // mapping kernel rodata -|-|R|V
    va += sz;
    pa += sz;
    sz = (unsigned long)_sdata - (unsigned long)_srodata;
    create_mapping(swapper_pg_dir, va, pa, sz, 0b0011);

    // mapping other memory -|W|R|V
    va += sz;
    pa += sz;
    sz = 0x80000000 - ((unsigned long)_sdata - (unsigned long)_stext);
    create_mapping(swapper_pg_dir, va, pa, sz, 0b0111);

    // set satp with swapper_pg_dir
    unsigned long p = (unsigned long)swapper_pg_dir - PA2VA_OFFSET;
    __asm__ volatile(
        "li t0, 0b1000\n"
        "slli t0, t0, 60\n"
        "mv t1, %[p]\n"
        "srli t1, t1, 12\n"
        "add t0, t0, t1\n"
        "csrw satp, t0"
        :
        :[p]"r"(p)
        :"memory"
    );

    // flush TLB
    asm volatile("sfence.vma zero, zero");
    return;
}