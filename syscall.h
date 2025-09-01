#ifndef SYSCALL_H
#define SYSCALL_H
#include <stdint.h>
#define SYS_SLEEP 5
#define SYS_GETPID 6
#define SYS_WAIT   7



#define SYS_WRITE    1
#define SYS_READ     2
#define SYS_GETCHAR  3

void syscall_init();
uint64_t syscall_handler(uint64_t number, uint64_t arg1, uint64_t arg2, uint64_t arg3);

#endif
