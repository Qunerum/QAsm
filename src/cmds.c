#include <stdio.h>
#include "../include/cmds.h"

static FILE* f;
void setF(FILE* file) { f = file; }

void prt(char* args) { fprintf(f, "    lea rsi, [rel vstr_%s]\n    mov rdx, len_%s\n    call prt\n", args, args); } // Print string
void prti(char* args) { fprintf(f, "    mov eax, [rel vint_%s]\n    call intToText\n    call prt\n", args); } // Print int
void prtln(char* args) { fprintf(f, "    lea rsi, [rel nl]\n    mov rdx, nll\n    call prt\n"); } // Print line

qaCmd cmds[] = {
    {"prt", prt},
    {"prti", prti},
    {"prtln", prtln},
};
int cmd_count = sizeof(cmds) / sizeof(qaCmd);
