#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "framebuffer.c"
#include "keyboard.h"
#include "serial_port.c"
#include "paging.h"

extern u32int phyaddtop;
extern pagedir_t *cdir;
struct IDTDescriptor idt_descriptors[256];
struct IDT idt;

unsigned int BUFFER_COUNT=0;

void interrupts_init_descriptor(int index, unsigned int address)
{
	idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; 
	idt_descriptors[index].offset_low = (address & 0xFFFF); 

	idt_descriptors[index].segment_selector = 0x08; 
	idt_descriptors[index].reserved = 0x00; 

	
	idt_descriptors[index].type_and_attr =	(0x01 << 7)|(0x00 << 6)|(0x00 << 5)|(0x0e);
}

void interrupts_install_idt()
{
	interrupts_init_descriptor(33, (unsigned int) interrupt_handler_33);
	interrupts_init_descriptor(14, (unsigned int) interrupt_handler_14);


	idt.address = (int) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) *256;
	load_idt((int) &idt);


	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
}



void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, unsigned int interrupt, __attribute__((unused)) struct stack_state stack)
{
	unsigned char scan_code;
	unsigned char ascii;

	switch (interrupt){
		case 33:

			scan_code = keyboard_read_scan_code();

			if (scan_code <= 83) {
				ascii = keyboard_scan_code_to_ascii(scan_code);
				char str[1];
				str[0] = ascii;
				if(scan_code == 14 ){
					BUFFER_COUNT--;
					fb_clear(BUFFER_COUNT);

				}
				else{
					fb_write(str[0],BUFFER_COUNT);
					serial_write(0x3f8,str[0]);
					BUFFER_COUNT++;
				}
			}

			pic_acknowledge(interrupt);

			break;
			case 14:
				fb_write('l',1);
				u32int i=0;
				while(i<phyaddtop)
					{
 
    					alloc_frame(get_page(i,1,cdir),0,0);
    					i+=0x1000;

					}

		default:
			break;
    }
}
