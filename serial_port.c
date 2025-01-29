#include "serial_port.h"
unsigned short divi=1;
unsigned short com=0x3f8; 
void serial_configure_baudrate(unsigned short com,unsigned short div)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),0x80);
    outb(SERIAL_DATA_PORT(com),((div<<8)&0x00ff));
    outb(SERIAL_DATA_PORT(com),(div&0x00ff));
}
void serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),0x03);
}
void serial_configure_fifo_buffer(unsigned short com)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com),0xc7);
}
void serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com),0x03);
}
int serial_is_transmit_fifo_empty(unsigned short com)
{
    return (inb(SERIAL_LINE_STATUS_PORT(com))&0x20);
}
int serial_write(unsigned short com,char buf)
{
    serial_configure(com,divi);
    //unsigned int ibf=0;
    //while(ibf<len)
    //{ 
        if(serial_is_transmit_fifo_empty(com)){
            serial_write_byte(com,buf);
            //ibf++;
            
            //ibf++;

        }
    //}
    return 0;
}
int serial_writeword(unsigned short com,char *buf,int len)
{
    serial_configure(com,divi);
     int ibf=0;
    while(ibf<len)
    { 
        if(serial_is_transmit_fifo_empty(com)){
            serial_write_byte(com,buf[ibf]);
            ibf++;
            
            //ibf++;

        }
    }
    return 0;
}
void serial_write_byte(unsigned short com,char bytedata)
{
    outb(com,bytedata);
}
void serial_configure(unsigned short com,unsigned short baudrate)
{
    serial_configure_baudrate(com,baudrate);
    serial_configure_line(com);
    serial_configure_fifo_buffer(com);
    serial_configure_modem(com);


}