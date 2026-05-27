#ifndef CMDS_H
#define CMDS_H

#include <stdio.h>

typedef struct {
    char* cmd;
    void (*data)(char*);
} qaCmd;
extern qaCmd cmds[];
extern int cmd_count;

void setF(FILE* file);

#endif
