#ifndef DEVICES_H
#define DEVICES_H

void vga_print(const char* str, int row);
void keyboard_init();
char keyboard_get_char(); // blocks until key pressed
void pic_remap();
void keyboard_interrupt_handler();

#endif

#ifndef DEVICES_H
#define DEVICES_H
#include <stdint.h>

void keyboard_init();
char keyboard_get_char();
void vga_print(const char* str, int row);

#endif
