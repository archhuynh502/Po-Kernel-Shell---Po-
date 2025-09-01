#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(int n, uint64_t handler){
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08; 
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].base_mid = (handler >> 16) & 0xFFFF;
    idt[n].base_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].reserved = 0;
}

void idt_load(){
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;
    asm volatile("lidt %0" : : "m"(idtp));
}
