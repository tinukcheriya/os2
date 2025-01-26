#ifndef INCLUDE_SERIAL_PORT_H
#define INCLUDE_SERIAL_PORT_H
#pragma once
#include "io.h"

#define SERIAL_COM1_BASE 0x3f8
#define SERIAL_DATA_PORT(base) (base) 
#define SERIAL_FIFO_COMMAND_PORT(base) (base+2)
#define SERIAL_LINE_COMMAND_PORT(base) (base+3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base+4)
#define SERIAL_LINE_STATUS_PORT(base) (base+5)
extern unsigned short divi; 
void serial_configure_baudrate(unsigned short com,unsigned short divisor);
void serial_configure_line(unsigned short com);
void serial_configure_fifo_buffer(unsigned short com);
void serial_configure_modem(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned short com);
int serial_write(unsigned short com,char buf);
void serial_write_byte(unsigned short com,char bytedata);
void serial_configure(unsigned short com,unsigned short baudrate);

#endif