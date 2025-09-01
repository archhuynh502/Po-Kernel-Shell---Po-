#include "devices.h"
#include "memory.h"
#include "process.h"
#include "idt.h"
#include "fs.h"



void process_entry0();
void process_entry1();

void kernel_main(void){
    memory_init();
    process_init();
    idt_load();
    keyboard_init();

    // stack allocation device thingy... the only stack im allocating is stack overflow allocation on the memdisk.
    for(int i=0;i<MAX_PROCESSES;i++){
        process_table[i].rsp = (uint64_t)create_stack(16*1024);
        process_table[i].rip = (uint64_t)(i==0 ? process_entry0 : process_entry1);
        process_table[i].active = 1;
    }

    asm volatile("sti"); // enable interrupts
    ((void(*)())process_table[0].rip)(); // jump to first process
    for(;;) asm volatile("hlt"); // fxck your cpu 
}
extern uint8_t disk_image[]; // GO... LOOK.. AT THE EFFIN DISK IMAGE
fs_init(disk_image);

struct ext2_superblock sb;
read_superblock(&sb);
// you can now read sb.s_blocks_count, sb.s_inodes_count, etc.


struct ext2_inode file_inode;
read_inode(2, &file_inode); 
uint8_t file_data[1024];
read_file(&file_inode, file_data, 1024);
vga_print((char*)file_data, 5);



// test procesdses
void process_entry0(){
    while(1){
        vga_print("test 0 ", 0);
    }
}

void process_entry1(){
    while(1){
        vga_print("test 1 for measure incase test 0 just managed to get thru security ", 1);
    }
}
void print_entry(const char* name, uint32_t inode){
    vga_print(name, 10); // prints each entry at row 10
}

struct ext2_inode root_inode;
read_inode(2, &root_inode); // inode 2 = root
ext2_list_dir(&root_inode, print_entry);

uint32_t file_inode_num = ext2_lookup(&root_inode, "hello.txt");
struct ext2_inode file_inode;
read_inode(file_inode_num, &file_inode);
uint8_t file_data[1024];
read_file(&file_inode, file_data, 1024);
vga_print((char*)file_data, 20);

uint32_t file_inode_num = ext2_lookup(&root_inode, "bigfile.txt");
struct ext2_inode file_inode;
read_inode(file_inode_num, &file_inode);

uint8_t* file_data = kmalloc(file_inode.i_size);
read_file(&file_inode, file_data, file_inode.i_size);
vga_print((char*)file_data, 30);

void kernel_main(){
    memory_init();
    process_init();
    idt_load();
    pit_init();
    keyboard_init();
    syscall_init();

    // allocate process stacks
    for(int i=0;i<MAX_PROCESSES;i++){
        process_table[i].rsp = (uint64_t)create_stack(16*1024);
        process_table[i].rip = (uint64_t)(i==0 ? process_entry0 : process_entry1);
        process_table[i].active = 1;
    }

    asm volatile("sti"); // enable interrupts
    while(1) asm volatile("hlt");
}
void process_entry0(){
    while(1){
        char c = keyboard_get_char();
        if(c) vga_print(&c, 10);
    }
}
void process_entry0(){
    int pid = syscall_fork();
    if(pid==0){
        // child process
        vga_print("Child process running", 10);
    }else{
        // parent process
        vga_print("Parent process running", 5);
    }
    while(1) asm volatile("hlt");
}

static inline int syscall_fork(){
    int ret;
    asm volatile(
        "mov rax, %1\n"
        "int 0x80\n"
        "mov %0, rax"
        : "=r"(ret)
        : "r"(SYS_FORK)
        : "rax"
    );
    return ret;
}

static inline void syscall_sleep(uint32_t ticks){
    asm volatile(
        "mov rax, %0\n"
        "mov rdi, %1\n"
        "int 0x80\n"
        :
        : "r"(SYS_SLEEP), "r"(ticks)
        : "rax","rdi"
    );
}

void process_entry0(){
    while(1){
        vga_print("Ticking...", 5);
        syscall_sleep(100); // sleep for 100 ticks
    }
}
static inline int syscall_getpid(){
    int ret;
    asm volatile(
        "mov rax, %1\n"
        "int 0x80\n"
        "mov %0, rax"
        : "=r"(ret)
        : "r"(SYS_GETPID)
        : "rax"
    );
    return ret;
}

static inline void syscall_wait(int pid){
    asm volatile(
        "mov rax, %0\n"
        "mov rdi, %1\n"
        "int 0x80\n"
        :
        : "r"(SYS_WAIT), "r"(pid)
        : "rax","rdi"
    );
}

void process_entry0(){
    int pid = syscall_fork();
    if(pid==0){
        vga_print("Child running", 10);
    }else{
        vga_print("Parent waiting for child", 5);
        syscall_wait(pid);
        vga_print("Child finished", 6);
    }
    while(1) asm volatile("hlt");
}
void kernel_main(){
    memory_init();
    process_init();
    idt_load();
    pit_init();
    keyboard_init();
    syscall_init();

    // create process stacks & initialize PML4 if needed
    for(int i=0;i<MAX_PROCESSES;i++){
        process_table[i].rsp = (uint64_t)create_stack(16*1024);
        process_table[i].rip = (uint64_t)(i==0 ? process_entry0 : process_entry1);
        process_table[i].active = 1;
        process_table[i].pml4 = kernel_pml4; // assign page table for now
    }

    asm volatile("sti");
    while(1) asm volatile("hlt");
}
void kernel_main(){
    memory_init();
    process_init();
    idt_load();
    pit_init();
    keyboard_init();
    syscall_init();

    for(int i=0;i<MAX_PROCESSES;i++){
        process_table[i].rsp = (uint64_t)create_stack(16*1024);
        process_table[i].rip = (uint64_t)(i==0 ? process_entry0 : process_entry1);
        process_table[i].active = 1;
        process_table[i].pml4 = create_process_page_table();
    }

    asm volatile("sti");
    while(1) asm volatile("hlt");
}
