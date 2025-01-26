#include "framebuffer.h"
char *fb=(char *)0xb8000;

void fb_move_cursor(unsigned short pos)
{

    outb(0x3D4,14);
    outb(0x3D5,((pos<<8)&0xFF));
    outb(0x3D4,15);
    outb(0x3D5,(pos&0xFF));

}
void fb_write_cell(unsigned int i,char c)
{
    fb[2*i]=c;
    fb[2*i+1]=0x0f;
}
void fb_write(char c,unsigned int len)
{
    
    fb_write_cell(len,c);
    fb_move_cursor(len);
    
    
    

 
}

void fb_clear(unsigned int i){

  fb_write_cell(i, ' ');
  
}

