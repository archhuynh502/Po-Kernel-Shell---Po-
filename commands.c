#include "commands.h"
#include "fs.h"
#include "devices.h"
#include <string.h>

void cmd_echo(char* args){
    vga_print(args, 1);
    vga_print("\n",1);
}

void cmd_ls(char* path){
    if(!path || path[0]==0) path="/";
    ext2_ls(path);
}

void cmd_cat(char* filename){
    if(!filename || filename[0]==0) return;
    ext2_cat(filename);
}

void cmd_help(){
    vga_print("Available commands:\n",1);
    vga_print("echo [text]\n",1);
    vga_print("ls [path]\n",1);
    vga_print("cat [file]\n",1);
    vga_print("help\n",1);
}
#ifndef COMMANDS_H
#define COMMANDS_H

void cmd_echo(char* args);
void cmd_ls(char* path);
void cmd_cat(char* filename);
void cmd_mkdir(char* path);
void cmd_rm(char* path);
void cmd_touch(char* filename);
void cmd_clear();
void cmd_pwd();
void cmd_cd(char* path);
void cmd_run(char* program);
void cmd_sleep(int ticks);
void cmd_getpid();
void cmd_help();

#endif
#include "commands.h"
#include "fs.h"
#include "devices.h"
#include "process.h"
#include "syscall.h"
#include <string.h>

void cmd_echo(char* args){ vga_print(args,1); vga_print("\n",1);}
void cmd_ls(char* path){ ext2_ls(path); }
void cmd_cat(char* filename){ ext2_cat(filename); }
void cmd_mkdir(char* path){ ext2_mkdir(path); }
void cmd_rm(char* path){ ext2_rm(path); }
void cmd_touch(char* filename){ ext2_touch(filename); }
void cmd_clear(){ vga_clear(); }
void cmd_pwd(){ vga_print(current_working_dir,1); vga_print("\n",1); }
void cmd_cd(char* path){ ext2_cd(path); }
void cmd_run(char* program){ syscall_run(program); }
void cmd_sleep(int ticks){ syscall_sleep(ticks); }
void cmd_getpid(){ char buf[16]; itoa(syscall_getpid(),buf,10); vga_print(buf,1); vga_print("\n",1); }
void cmd_help(){
    vga_print("Available commands:\n",1);
    vga_print("echo [text]\nls [path]\ncat [file]\nmkdir [dir]\nrm [file/dir]\ntouch [file]\nclear\npwd\ncd [dir]\nrun [program]\nsleep [ticks]\ngetpid\nhelp\n",1);
}

