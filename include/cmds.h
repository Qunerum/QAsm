#ifndef CMDS_H
#define CMDS_H

typedef struct {
    char* cmd;
    char* data;
} qaCmd;
extern qaCmd cmds[];
extern int cmd_count;

#endif
