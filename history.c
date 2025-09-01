#include "history.h"
#include <string.h>

static char history[HISTORY_SIZE][MAX_CMD_LEN];
static int history_count = 0;
static int cursor = 0;

void history_add(const char* cmd){
    if(!cmd || cmd[0]==0) return;
    if(history_count < HISTORY_SIZE){
        strncpy(history[history_count++], cmd, MAX_CMD_LEN-1);
    }else{
        // shift history up
        for(int i=1;i<HISTORY_SIZE;i++) strncpy(history[i-1], history[i], MAX_CMD_LEN);
        strncpy(history[HISTORY_SIZE-1], cmd, MAX_CMD_LEN-1);
    }
    history_reset_cursor();
}

const char* history_prev(){
    if(history_count==0) return 0;
    if(cursor==0) cursor = history_count-1;
    else cursor--;
    return history[cursor];
}

const char* history_next(){
    if(history_count==0) return 0;
    if(cursor >= history_count-1) cursor = 0;
    else cursor++;
    return history[cursor];
}

void history_reset_cursor(){
    cursor = history_count;
}

