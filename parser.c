#include "parser.h"
#include "commands.h"
#include <string.h>
#include <stdint.h>

#define MAX_ARGS 8

static char* argv[MAX_ARGS];

void shell_parse(char* input){
    int argc=0;
    char* token = strtok(input," ");
    while(token && argc<MAX_ARGS){
        argv[argc++] = token;
        token = strtok(0," ");
    }

    if(argc==0) return;

    char* cmd = argv[0];
    char* args = argc>1 ? input + (argv[1]-input) : "";

    if(strcmp(cmd,"echo")==0) cmd_echo(args);
    else if(strcmp(cmd,"ls")==0) cmd_ls(argc>1 ? argv[1] : "/");
    else if(strcmp(cmd,"cat")==0) cmd_cat(argc>1 ? argv[1] : "");
    else if(strcmp(cmd,"mkdir")==0) cmd_mkdir(argc>1 ? argv[1] : "");
    else if(strcmp(cmd,"rm")==0) cmd_rm(argc>1 ? argv[1] : "");
    else if(strcmp(cmd,"touch")==0) cmd_touch(argc>1 ? argv[1] : "");
    else if(strcmp(cmd,"clear")==0) cmd_clear();
    else if(strcmp(cmd,"pwd")==0) cmd_pwd();
    else if(strcmp(cmd,"cd")==0) cmd_cd(argc>1 ? argv[1] : "/");
    else if(strcmp(cmd,"run")==0) cmd_run(argc>1 ? argv[1] : "");
    else if(strcmp(cmd,"sleep")==0) cmd_sleep(argc>1 ? atoi(argv[1]) : 1);
    else if(strcmp(cmd,"getpid")==0) cmd_getpid();
    else if(strcmp(cmd,"help")==0) cmd_help();
    else vga_print("Unknown command\n",1);
}

