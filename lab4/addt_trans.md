A virtual address va is translated into a physical address pa as follows:

Let a be ${\tt satp}.ppn \times \textrm{PAGESIZE}$, and let i = LEVELS − 1. (For Sv32, PAGESIZE=212 and LEVELS=2.)

Let pte be the value of the PTE at address a + va.vpn[i] × PTESIZE. (For Sv32, PTESIZE=4.) If accessing pte violates a PMA or PMP check, raise an access-fault exception corresponding to the original access type.

If pte.v = 0, or if pte.r = 0 and pte.w = 1, stop and raise a page-fault exception corresponding to the original access type.

Otherwise, the PTE is valid. If pte.r = 1 or pte.x = 1, go to step 5. Otherwise, this PTE is a pointer to the next level of the page table. Let i = i − 1. If i < 0, stop and raise a page-fault exception corresponding to the original access type. Otherwise, let a = pte.ppn × PAGESIZE and go to step 2.

A leaf PTE has been found. Determine if the requested memory access is allowed by the pte.r, pte.w, pte.x, and pte.u bits, given the current privilege mode and the value of the SUM and MXR fields of the mstatus register. If not, stop and raise a page-fault exception corresponding to the original access type.

If i > 0 and pte.ppn[i − 1 : 0] ≠ 0, this is a misaligned superpage; stop and raise a page-fault exception corresponding to the original access type.

If pte.a = 0, or if the memory access is a store and pte.d = 0, either raise a page-fault exception corresponding to the original access type, or:

Set pte.a to 1 and, if the memory access is a store, also set pte.d to 1.

If this access violates a PMA or PMP check, raise an access-fault exception corresponding to the original access type.

This update and the loading of pte in step 2 must be atomic; in particular, no intervening store to the PTE may be perceived to have occurred in-between.

The translation is successful. The translated physical address is given as follows:

pa.pgoff = va.pgoff.

If i > 0, then this is a superpage translation and pa.ppn[i − 1 : 0] = va.vpn[i − 1 : 0].

pa.ppn[LEVELS − 1 : i] = pte.ppn[LEVELS − 1 : i].