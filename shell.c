#include "shell.h"
#include "parser.h"
#include "devices.h"
#include <string.h>

void shell_process(){
    char input[128];
    while(1){
        vga_print("> ",0);
        memset(input,0,sizeof(input));
        int pos=0;

        while(1){
            char c = keyboard_get_char();
            if(!c) continue;

            if(c=='\n'){
                input[pos]=0;
                vga_print("\n",0);
                break;
            }else if(c=='\b' && pos>0){
                pos--;
                vga_print("\b \b",0);
            }else{
                input[pos++] = c;
                char tmp[2]={c,0};
                vga_print(tmp,0);
            }
        }

        shell_parse(input);
    }
}
#include "history.h"

void shell_process(){
    char input[128];
    history_reset_cursor();

    while(1){
        vga_print("> ",0);
        memset(input,0,sizeof(input));
        int pos=0;

        while(1){
            char c = keyboard_get_char();
            if(!c) continue;

            if(c=='\n'){
                input[pos]=0;
                vga_print("\n",0);
                history_add(input);
                break;
            }else if(c=='\b' && pos>0){
                pos--;
                vga_print("\b \b",0);
            }else if(c==0xE0){ // arrow prefix for PS/2
                char arrow = keyboard_get_char();
                if(arrow==0x48){ // up
                    const char* prev = history_prev();
                    if(prev){
                        for(int i=0;i<pos;i++) vga_print("\b \b",0);
                        strncpy(input, prev, 128);
                        pos = strlen(prev);
                        vga_print(prev,0);
                    }
                }else if(arrow==0x50){ // down
                    const char* next = history_next();
                    if(next){
                        for(int i=0;i<pos;i++) vga_print("\b \b",0);
                        strncpy(input, next, 128);
                        pos = strlen(next);
                        vga_print(next,0);
                    }
                }
            }else{
                input[pos++] = c;
                char tmp[2]={c,0};
                vga_print(tmp,0);
            }
        }

        shell_parse(input);
    }
}

