#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

void memory_init();
void* kmalloc(uint64_t size);
void* create_stack(uint64_t size);

#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

#define PAGE_SIZE 4096

void memory_init();
uint64_t* create_page_table();
void map_page(uint64_t* pml4, uint64_t va, uint64_t pa, uint64_t flags);

#endif
