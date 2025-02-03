#include "paging.h"
#include "kheap.h"
#include "interrupts.h"
#include "framebuffer.h"

pagedir_t *kdir=0;
pagedir_t *cdir=0;

u32int *frames;
u32int nframes;

extern u32int phyaddtop;

#define index(a) a/32
#define offset(a) a%32

static void set_frame(u32int frameadd)
{
     u32int frame=frameadd/0x1000;
     u32int idx=index(frame);
     u32int off=offset(frame);
     frames[idx]|=(0x1<<off);
    
}
static void clear_frame(u32int frameadd)
{
    u32int frame=frameadd/0x1000;
    u32int idx=index(frame);
    u32int off=offset(frame);
    frames[idx]|=~(0x1<<off); 
}
static s32int first_frame()
{
    u32int i,j;
    for(i=0;i<=(index(nframes));i++)

    {
        if(frames[i]!=0xFFFFFFFF)
            {
        for(j=0;j<32;j++)
        {
            
                if(!((0x1<<j)&frames[i]))
                {
                    return (i*32+j);
                }
            }
        }
    }
    return -1;
}

void alloc_frame(page_t *page,u32int isKernel,u32int isWriteable)
{
    if(page->frame!=0)
    {
        return;
    }
    else{
        u32int idx;
        idx=first_frame();
        if(idx==(u32int)-1)
        {
            while(1);
        }
        fb_write('h',2);
        set_frame(idx*0x1000);
        page->present=1;
        page->rw=(isWriteable)?1:0;
        page->user=(isKernel)?0:1;
        page->frame=idx;
    }
}
void free_frame(page_t *page)
{
    u32int frame=page->frame;
    if(!frame)
    {
        return;
    }
    else{
        clear_frame(frame*0x1000);
        page->frame=0x0;
    }
}

void custom_memset(u8int *address,u32int val,u32int size)
{
    for(u32int i=0;i<size;i++){
    *address=val;
    ++address;
    }
}
void init_paging()
{
   //setphysicaladdresstop(kernalphysicalend); 

u32int mem_end=0x1000000;
nframes=mem_end/0x1000;
frames=(u32int *)kmalloc(index(nframes)+1);
custom_memset((u8int *)frames,0,index(nframes));
kdir=(pagedir_t *)kmalloc_a(sizeof(pagedir_t));
cdir=kdir;
if(!get_page(0,0,cdir))
asm volatile("int $0x0e");


switchpagedir(kdir);
}

page_t *get_page(u32int address,u8int make,pagedir_t *dir)
{
    address/=0x1000;
    u32int tableind = address/1024;
    if(dir->tables[tableind])
    {
        return &dir->tables[tableind]->pages[address%1024];
    }
    else if(make)
    {
      u32int tmp;
      dir->tables[tableind]=(pagetable_t*)kmalloc_ap(sizeof(pagetable_t),&tmp);
      custom_memset((u8int *)dir->tables[tableind],0,0x1000);
       dir->tablesaddr[tableind] =tmp |0x7;
       return &dir->tables[tableind]->pages[address%1024];
    }
    else{
        return 0;
    }

}
void switchpagedir(pagedir_t *new)
{
    cdir=new;
    asm volatile("mov %0,%%cr3"::"r"(&new->tablesaddr) );
    u32int cr0;
    asm volatile("mov %%cr0,%0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0,%%cr0"::"r"(cr0));


}


