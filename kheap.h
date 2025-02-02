#ifndef INCLUDE_KHEAP_H
#define INCLUDE_KHEAP_H
typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

//void setphysicaladdresstop(u32int physicaladdress);
void create_heap(u32int startaddr, u32int endaddr, u32int maxaddr);
u32int kmalloc_int(u32int size,u8int align,u32int *ptr);

u32int kmalloc_a(u32int size);

u32int kmalloc_p(u32int size,u32int *ptr);

u32int kmalloc_ap(u32int size,u32int *ptr);

u32int kmalloc(u32int size);
#endif