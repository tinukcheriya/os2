#ifndef INCLUDE_PAGING_H
#define INCLUDE_PAGING_H

#pragma once


typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

typedef struct page{
    u32int present : 1;
    u32int rw :1;
    u32int user :1;
    u32int accessed : 1;
    u32int dirty : 1;
    u32int unused : 7;
    u32int frame : 20;

}page_t;

typedef struct pagetable
{
    page_t pages[1024];
}pagetable_t;
typedef struct pagedir
{
    pagetable_t *tables[1024];
    u32int tablesaddr[1024];
    u32int phyaddre;
}pagedir_t;

void init_paging();
void switchpagedir(pagedir_t *new);
page_t *get_page(u32int address,u8int make,pagedir_t *dir);
#endif


