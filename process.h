#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>

typedef struct {
    uint64_t rip;
    uint64_t rsp;
    uint64_t registers[16];
    int active;
} pcb_t;

#define MAX_PROCESSES 4
extern pcb_t process_table[MAX_PROCESSES];
extern int current_process;

void process_init();
void switch_process();
void scheduler_tick();

#endif

#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include "fs.h"

#define MAX_PROCESSES 16
#define MAX_FD 16

struct file_desc {
    struct ext2_inode* inode;
    uint32_t offset;
    uint8_t used;
};

struct pcb {
    uint64_t rip;
    uint64_t rsp;
    uint8_t active;
    struct file_desc fd_table[MAX_FD];
};

extern struct pcb process_table[MAX_PROCESSES];
void process_init();
#endif
int process_fork(struct pcb* parent);
void process_sleep(struct pcb* p, uint32_t ticks);
void scheduler_tick();

int process_getpid(struct pcb* p);
int process_wait(struct pcb* p, int child_pid);
#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>

struct pcb {
    uint64_t rip;
    uint64_t rsp;
    uint8_t active;
    uint64_t* pml4;
    uint32_t sleep_ticks;
    // file descriptors, etc.
};

extern struct pcb process_table[];
extern struct pcb* current_process;

uint64_t* create_page_table();
void switch_process_memory(struct pcb* p);

#endif
