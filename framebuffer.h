#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H
#include "io.h"
void fb_move_cursor(unsigned short pos);
void fb_write_cell(unsigned int i,char c);
void fb_write(char c,unsigned int len);
void fb_clear(unsigned int i);
#endif