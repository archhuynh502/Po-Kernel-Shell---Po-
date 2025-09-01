void pit_init(){
    uint32_t divisor = 1193180 / 100; // 100Hz
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}
