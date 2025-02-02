#include "kheap.h"
#include "paging.h"

#define NULL ((void*)0)


#define HEAP_MAGIC 0x12345678

typedef struct {
    u32int start;
    u32int end;
    u32int max;
    u32int supervisor;
    u32int readonly;
} heap_t;

typedef struct header {
    u32int magic;
    u8int isHole;
    u32int size;
} header_t;

typedef struct hole_node {
    header_t* header;
    struct hole_node* next;
} hole_node_t;

u32int phyaddtop = 0;
heap_t *heap = 0;
hole_node_t* free_list = NULL;

void create_heap(u32int startaddr, u32int endaddr, u32int maxaddr) {
    heap = (heap_t*)startaddr;
    heap->start = startaddr + sizeof(heap_t);
    heap->end = endaddr;
    heap->max = maxaddr;

    
    header_t* hole = (header_t*)heap->start;
    hole->magic = HEAP_MAGIC;
    hole->isHole = 1;
    hole->size = heap->end - heap->start - sizeof(header_t);

    
    hole_node_t* node = (hole_node_t*)(heap->start + sizeof(header_t));
    node->header = hole;
    node->next = NULL;
    free_list = node;

    phyaddtop = heap->start + sizeof(header_t) + hole->size;
}

void add_to_free_list(header_t* header) {
    hole_node_t* node = (hole_node_t*)((u32int)header + sizeof(header_t));
    node->header = header;
    node->next = free_list;
    free_list = node;
}

void remove_from_free_list(header_t* header) {
    hole_node_t* current = free_list;
    hole_node_t* previous = NULL;

    while (current != NULL) {
        if (current->header == header) {
            if (previous == NULL) {
                free_list = current->next;
            } else {
                previous->next = current->next;
            }
            break;
        }
        previous = current;
        current = current->next;
    }
}

u32int kmalloc_int(u32int size, u8int align, u32int *ptr) {
    u32int newsize = size + sizeof(header_t);
    hole_node_t* current = free_list;

    while (current != NULL) {
        header_t* header = current->header;

        if (header->isHole && header->size >= newsize) {
            
            header->isHole = 0;
            remove_from_free_list(header);

             

            
            if (header->size > newsize + sizeof(header_t)) {
                header_t* newhole = (header_t*)((u32int)header + sizeof(header_t) + newsize);
                newhole->magic = HEAP_MAGIC;
                newhole->isHole = 1;
                newhole->size = header->size - newsize - sizeof(header_t);

                add_to_free_list(newhole);
            }
            if (align) {
                u32int offset = ((u32int)header + sizeof(header_t)) & 0xFFF;
                if (offset) {
                    u32int alignoff = 0x1000 - offset;
                    header = (header_t*)((u32int)header + alignoff);
                    header->size = newsize;
                }
            }

            
           

            if (ptr) {
                *ptr = (u32int)header + sizeof(header_t);
            }

            phyaddtop = (u32int)header + sizeof(header_t) + newsize;
            return (u32int)header + sizeof(header_t);
        }

        current = current->next;
    }

  
    return 0;
}

void kfree(u32int ptr) {
    if (ptr == 0) return;

    header_t* header = (header_t*)(ptr - sizeof(header_t));
    if (header->magic == HEAP_MAGIC) {
        header->isHole = 1;
        add_to_free_list(header);

     
        hole_node_t* current = free_list;
        while (current != NULL) {
            header_t* current_header = current->header;

            if (current_header->isHole) {
                header_t* next_header = (header_t*)((u32int)current_header + sizeof(header_t) + current_header->size);

                if ((u32int)next_header < heap->end && next_header->isHole) {
                    current_header->size += sizeof(header_t) + next_header->size;
                    remove_from_free_list(next_header);
                }
            }

            current = current->next;
        }
    }
}

u32int kmalloc_a(u32int size) {
    return kmalloc_int(size, 1, 0);
}

u32int kmalloc_p(u32int size, u32int *ptr) {
    return kmalloc_int(size, 0, ptr);
}

u32int kmalloc_ap(u32int size, u32int *ptr) {
    return kmalloc_int(size, 1, ptr);
}

u32int kmalloc(u32int size) {
    return kmalloc_int(size, 0, 0);
}