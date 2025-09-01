#ifndef HISTORY_H
#define HISTORY_H

#define HISTORY_SIZE 16
#define MAX_CMD_LEN 128

void history_add(const char* cmd);
const char* history_prev();
const char* history_next();
void history_reset_cursor();

#endif

