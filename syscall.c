#include <stdint.h>
#define SYS_FORK 4


static inline uint64_t sys_write(const char* str, int row){
    uint64_t ret;
    asm volatile(
        "mov rax, %1\n"
        "mov rdi, %2\n"
        "mov rsi, %3\n"
        "int 0x80\n"
        "mov %0, rax"
        : "=r"(ret)
        : "r"(SYS_WRITE), "r"(str), "r"(row)
        : "rax","rdi","rsi"
    );
    return ret;
}

static inline char sys_getchar(){
    uint64_t ret;
    asm volatile(
        "mov rax, %1\n"
        "int 0x80\n"
        "mov %0, rax"
        : "=r"(ret)
        : "r"(SYS_GETCHAR)
        : "rax"
    );
    return (char)ret;
}

// the fxcking example what do you think
void process_entry0(){
    while(1){
        sys_write("testing...", 5);
        char c = sys_getchar();
        if(c) sys_write(&c, 6);
    }
}
case SYS_FORK:
    return process_fork(current_process);
case SYS_GETPID:
    return process_getpid(current_process);
case SYS_WAIT:
    return process_wait(current_process, (int)arg1);
