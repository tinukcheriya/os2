//#include "framebuffer.h"
//#include "serial_port.h"
#include "segmentation.h"
//#include "keyboard.h"
#include "interrupts.h"
void kmain()
{ 
    //char msg[4]="MYOS";
    //for(int i=0;i<4;i++)
    //{
       //fb_write(msg[i],i);
       
    //}
    //serial_write(0x3f8,msg,4);
    //unsigned char scancode,ascii;
    //char asciicode[0];
    
    
    segments_install_gdt();
    interrupts_install_idt();
    //scancode = keyboard_read_scan_code();
    //ascii = keyboard_scan_code_to_ascii(scancode);
    //asciicode[0] = ascii;
    
}

