#include "../include/cmds.h"

qaCmd cmds[] = {
    {"print", "lea rsi, [rel ;a;]"}
    // lea rsi, [rel pl1t]
    // mov rdx, pl1l
    // call prt ; println a is
};
int cmd_count = sizeof(cmds) / sizeof(qaCmd);
