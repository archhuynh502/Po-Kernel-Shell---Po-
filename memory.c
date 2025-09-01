#include "memory.h"
// note to all programmers who touched this
// time wasted : 32 hours
// also this uses 1024 byte = kilobyte instead of the 1000 = 1 KB byte system :) 
#define HEAP_SIZE (1024*1024)  // one megabyte kernel heaps yayyy
#define STACK_SIZE (16*1024)   // 160000 per process (16 KB i think), 

static uint8_t heap[HEAP_SIZE];
static uint64_t heap_index = 0;

void memory_init(){
    heap_index = 0;
}

void* kmalloc(uint64_t size){
    if(heap_index + size > HEAP_SIZE) return 0; //  LULZ your PC has low specs
    void* ptr = &heap[heap_index];
    heap_index += size;
    return ptr;
}

void* create_stack(uint64_t size){
    void* stack = kmalloc(size);
    if(!stack) return 0;
    return (uint8_t*)stack + size; // stack grows down
}

#include "memory.h"
#include <string.h>

static uint64_t* kernel_pml4;

void memory_init(){
    kernel_pml4 = create_page_table();
    // Identity map first 16 MB
    for(uint64_t addr=0; addr<16*1024*1024; addr+=PAGE_SIZE){
        map_page(kernel_pml4, addr, addr, 0x03); // present + rw
    }
    asm volatile("mov cr3, %0"::"r"(kernel_pml4));
    uint64_t cr0;
    asm volatile("mov %%cr0,%0":"=r"(cr0));
    cr0 |= 0x80000000; // enable paging
    asm volatile("mov %0,%%cr0"::"r"(cr0));
}

uint64_t* create_page_table(){
    uint64_t* table = (uint64_t*)0x100000; // simple fixed location for demo
    memset(table, 0, PAGE_SIZE);
    return table;
}

void map_page(uint64_t* pml4, uint64_t va, uint64_t pa, uint64_t flags){
    pml4[va>>39 & 0x1FF] = (uint64_t)0x200000 | flags; // demo: point to a fixed PDPT
    // for brevity we skip full 4-level mapping
}
