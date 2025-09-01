#include "devices.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

static char keyboard_buffer[256];
static int kb_head=0, kb_tail=0;

static char scancode_to_ascii[128]={0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '};

static inline uint8_t inb(uint16_t port){ uint8_t ret; asm volatile("inb %1,%0":"=a"(ret):"Nd"(port)); return ret; }
static inline void outb(uint16_t port,uint8_t val){ asm volatile("outb %0,%1"::"a"(val),"Nd"(port)); }

void pic_remap(){
    uint8_t a1=inb(PIC1_DATA), a2=inb(PIC2_DATA);
    outb(PIC1_COMMAND,0x11);
    outb(PIC2_COMMAND,0x11);
    outb(PIC1_DATA,0x20);
    outb(PIC2_DATA,0x28);
    outb(PIC1_DATA,4);
    outb(PIC2_DATA,2);
    outb(PIC1_DATA,0x01);
    outb(PIC2_DATA,0x01);
    outb(PIC1_DATA,a1);
    outb(PIC2_DATA,a2);
}

extern void idt_set_gate(int n,uint64_t handler);

void keyboard_interrupt_handler();
void keyboard_init(){
    pic_remap();
    idt_set_gate(33,(uint64_t)keyboard_interrupt_handler);
    outb(PIC1_DATA, inb(PIC1_DATA)&~2); // enable IRQ1
}

char keyboard_get_char(){
    while(kb_tail==kb_head) asm volatile("hlt");
    char c=keyboard_buffer[kb_tail++];
    kb_tail&=0xFF;
    return c;
}

void keyboard_interrupt_handler(){
    uint8_t sc=inb(KEYBOARD_DATA_PORT);
    if(sc<128){
        char c=scancode_to_ascii[sc];
        if(c){ keyboard_buffer[kb_head++]=c; kb_head&=0xFF; }
    }
    outb(PIC1_COMMAND,PIC_EOI);
    asm volatile("iretq");
}

void vga_print(const char* str,int row){
    char* video=(char*)0xB8000; int i=0;
    while(str[i]){ video[(row*80+i)*2]=str[i]; video[(row*80+i)*2+1]=0x0F; i++; }
}

extern void timer_interrupt_handler();
void timer_init(){
    pic_remap();
    idt_set_gate(32,(uint64_t)timer_interrupt_handler);
    outb(0x40, 0x36); // PIT mode
    asm volatile("sti");
}
#include "devices.h"
#include <stdint.h>

#define DATA_PORT 0x60
#define STATUS_PORT 0x64

static volatile char kb_buffer[256];
static volatile int kb_head=0, kb_tail=0;

static char scancode_to_ascii(uint8_t sc){
    // minimal US QWERTY, letters only
    if(sc>=0x1E && sc<=0x26) return 'a'+sc-0x1E;
    if(sc>=0x2C && sc<=0x32) return 'z'+sc-0x2C;
    return 0;
}

void keyboard_isr(){
    uint8_t sc = inb(DATA_PORT);
    char c = scancode_to_ascii(sc);
    if(c){
        kb_buffer[kb_head++] = c;
        if(kb_head>=256) kb_head=0;
    }
    outb(0x20,0x20); // EOI
}

void keyboard_init(){
    // IRQ1 enabled in PIC elsewhere
}

char keyboard_get_char(){
    if(kb_tail==kb_head) return 0;
    char c = kb_buffer[kb_tail++];
    if(kb_tail>=256) kb_tail=0;
    return c;
}

// Minimal VGA output
volatile char* vga = (char*)0xB8000;
void vga_print(const char* str, int row){
    for(int i=0; str[i]; i++){
        vga[row*160 + i*2] = str[i];
        vga[row*160 + i*2 +1] = 0x0F;
    }
}

// minimal inb/outb
static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile ("inb %1,%0":"=a"(ret):"Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val){
    asm volatile ("outb %0,%1"::"a"(val),"Nd"(port));
}
