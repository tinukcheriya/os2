#include "framebuffer.h"
#include "serial_port.h"
#include "segmentation.h"
//#include "keyboard.h"
#include "interrupts.h"
#include "paging.h"
#include "multiboot.h"
static unsigned int read_cr3() {
    unsigned int value;
    asm volatile ("mov %%cr3, %0" : "=r" (value));
    return value;
}
void int_to_hex(unsigned int value, char *buffer) {
    const char hex_chars[] = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';  // Prefix for hex representation
    for (int i = 7; i >= 0; i--) {
        buffer[2 + i] = hex_chars[value & 0xF];  // Extract last 4 bits
        value >>= 4;  // Shift right by 4 bits
    }
    buffer[10] = '\0'; // Null-terminate the string
}

void kmain(unsigned int kernelend)
{ 
    //char msg[4]="MYOS";
    //for(int i=0;i<4;i++)
    //{
       //fb_write(msg[i],i);
       
    //}
    //serial_write(0x3f8,msg,4);
    //unsigned char scancode,ascii;
    //char asciicode[0];
    
    //unsigned char a[8];
    segments_install_gdt();
    init_paging(kernelend);

    unsigned int y=read_cr3();
    char hex_str[11];  // Buffer for "0xXXXXXXXX" + NULL

    int_to_hex(y, hex_str);

      serial_writeword(0x3f8,hex_str,10);
      volatile unsigned int *p=(unsigned int*)0x0000000;
      unsigned int valu=*p;
      char hex_sr[11];
      int_to_hex(valu, hex_sr);
      serial_writeword(0x3f8,hex_sr,10);
    ///for(int j=0;j<7;j++)
    //{
     // y=y>>(7-j)&0x0f;
      // a[j]=y;
      
    //}
    //for(int i=0;i<7;i++)
      //fb_write(a[i],i);


    

    interrupts_install_idt();
    //scancode = keyboard_read_scan_code();
    //ascii = keyboard_scan_code_to_ascii(scancode);
    //asciicode[0] = ascii;
    //multiboot_info_t *mbinfo=(multiboot_info_t *)ebx;
    //multiboot_module_t *modules =(multiboot_module_t *)mbinfo->mods_addr;
    //unsigned int address_of_module = modules->mod_start;
    //if((mbinfo->mods_count) == 1){
  		//serial_write(0x3F8,'o');
        //typedef void (*call_module_t)(void);
        //call_module_t start_program = (call_module_t) address_of_module;
        //start_program();
        //}
    //else{
  		
  		//serial_write(0x3F8,'E');
  	//}
    
}

