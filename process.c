#include "process.h"
#include <stdint.h>

pcb_t process_table[MAX_PROCESSES];
int current_process = 0;

void process_init(){
    for(int i=0;i<MAX_PROCESSES;i++){
        process_table[i].active = 0;
        process_table[i].rip = 0;
        process_table[i].rsp = 0;
    }
}

extern void save_context(pcb_t* pcb);
extern void load_context(pcb_t* pcb);

void switch_process(){
    save_context(&process_table[current_process]);
    do {
        current_process = (current_process+1) % MAX_PROCESSES;
    } while(!process_table[current_process].active);
    load_context(&process_table[current_process]);
}

void scheduler_tick(){
    switch_process();
}

#include "process.h"
#include <string.h>

struct pcb process_table[MAX_PROCESSES];

void process_init(){
    memset(process_table, 0, sizeof(process_table));
    for(int i=0;i<MAX_PROCESSES;i++){
        for(int j=0;j<MAX_FD;j++){
            process_table[i].fd_table[j].used=0;
        }
    }
}

int process_open(struct pcb* p, struct ext2_inode* inode){
    for(int i=0;i<MAX_FD;i++){
        if(!p->fd_table[i].used){
            p->fd_table[i].inode=inode;
            p->fd_table[i].offset=0;
            p->fd_table[i].used=1;
            return i;
        }
    }
    return -1;
}

int process_read(struct pcb* p, int fd, uint8_t* buffer, uint32_t size){
    if(fd<0 || fd>=MAX_FD || !p->fd_table[fd].used) return -1;
    struct file_desc* f = &p->fd_table[fd];
    read_file(f->inode, buffer+f->offset, size);
    f->offset+=size;
    return size;
}

int process_close(struct pcb* p, int fd){
    if(fd<0 || fd>=MAX_FD || !p->fd_table[fd].used) return -1;
    p->fd_table[fd].used=0;
    return 0;
}

#include "process.h"

struct pcb* current_process = 0;
uint32_t sleep_ticks;

int current_index = 0;

void context_switch(){
    if(!current_process) current_process = &process_table[0];
    current_index = (current_index + 1) % MAX_PROCESSES;
    for(int i=0;i<MAX_PROCESSES;i++){
        if(process_table[current_index].active){
            current_process = &process_table[current_index];
            asm volatile(
                "mov rsp, %0\n"
                "jmp *%1\n"
                :
                : "r"(current_process->rsp), "r"(current_process->rip)
            );
            break;
        }
        current_index = (current_index + 1) % MAX_PROCESSES;
    }
}
#include "process.h"
#include <string.h>

int process_fork(struct pcb* parent){
    for(int i=0;i<MAX_PROCESSES;i++){
        if(!process_table[i].active){
            process_table[i] = *parent; // copy PCB
            process_table[i].rsp = (uint64_t)create_stack(16*1024); // new stack
            memcpy((void*)process_table[i].rsp, (void*)parent->rsp, 16*1024); // copy stack
            process_table[i].active = 1;
            return i; // child PID
        }
    }
    return -1; // no free slot
}
#include "process.h"

static volatile uint32_t tick_count = 0;

void process_sleep(struct pcb* p, uint32_t ticks){
    p->active = 0;
    p->sleep_ticks = tick_count + ticks;
}

void scheduler_tick(){
    tick_count++;
    for(int i=0;i<MAX_PROCESSES;i++){
        if(!process_table[i].active && process_table[i].sleep_ticks <= tick_count){
            process_table[i].active = 1; // wake up
        }
    }
}
int process_getpid(struct pcb* p){
    for(int i=0;i<MAX_PROCESSES;i++){
        if(&process_table[i]==p) return i;
    }
    return -1;
}

int process_wait(struct pcb* p, int child_pid){
    if(child_pid<0 || child_pid>=MAX_PROCESSES) return -1;
    while(process_table[child_pid].active) asm volatile("hlt");
    return 0;
}
#include "process.h"
#include "memory.h"

struct pcb process_table[MAX_PROCESSES];
struct pcb* current_process = 0;
int current_index = 0;

void context_switch(){
    if(!current_process) current_process = &process_table[0];
    int start_index = current_index;
    do {
        current_index = (current_index + 1) % MAX_PROCESSES;
        if(process_table[current_index].active){
            current_process = &process_table[current_index];
            switch_process_memory(current_process);
            asm volatile(
                "mov rsp, %0\n"
                "jmp *%1\n"
                :
                : "r"(current_process->rsp), "r"(current_process->rip)
            );
            return;
        }
    } while(current_index != start_index);
}
