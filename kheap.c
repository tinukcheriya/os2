#include "kheap.h"

u32int phyaddtop=0;

void setphysicaladdresstop(u32int physicaladdress)
{
    phyaddtop=physicaladdress;
}

u32int kmalloc_int(u32int size,u32int align,u32int *ptr)
{
    if(align==1){
      if(phyaddtop&0x00000fff)
      {
         phyaddtop&=0xfffff000;
         phyaddtop+=0x1000;
      }
    }
    if(ptr)
    {
        *ptr=phyaddtop;
    }
    u32int tmp=phyaddtop;
    phyaddtop+=size;
    return tmp;
}

u32int kmalloc_a(u32int size)
{
    return kmalloc_int(size,1,0);
}
u32int kmalloc_p(u32int size,u32int *ptr)
{
    return kmalloc_int(size,0,ptr);
}
u32int kmalloc_ap(u32int size,u32int *ptr)
{
    return kmalloc_int(size,1,ptr);
}

u32int kmalloc(u32int size)
{
    return kmalloc_int(size,0,0);
}