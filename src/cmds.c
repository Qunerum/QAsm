#include <stdio.h>
#include "../include/main.h"
#include "../include/utility.h"
#include "../include/cmds.h"

static FILE* f;
void setF(FILE* file) { f = file; }
void split(char* data) { splitStart(data, ' ', ct[5], ct[6]); }

void startMain(char* args) { (void)args; fprintf(f, "_start:\n"); }
void endMain(char* args) { (void)args; fprintf(f, "\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n"); }

void step(char* args) { fprintf(f, "QAF_%s: ; Step\n", args); }
void es(char* args) { (void)args; fprintf(f, "\tret ; End step\n"); }
void call(char* args) { (void)args; fprintf(f, "\tcall QAF_%s ; Call step\n", args); }

void prt(char* args) { fprintf(f, "\tlea rsi, [rel vstr_%s]\n\tmov rdx, len_%s\n\tcall prt ; Print string\n", args, args); }
void prti(char* args) { fprintf(f, "\tmov eax, [rel vint_%s]\n\tcall intToText\n\tcall prt ; Print int\n", args); }
void prtln(char* args) { (void)args; fprintf(f, "\tlea rsi, [rel nl]\n\tmov rdx, nll\n\tcall prt ; Print line\n"); }

void add(char* args) { split(args); fprintf(f, "\tadd dword [rel vint_%s], %s ; Add\n", ct[5], ct[6]); }
void sub(char* args) { split(args); fprintf(f, "\tsub dword [rel vint_%s], %s ; Sub\n", ct[5], ct[6]); }
void mul(char* args) { split(args); fprintf(f, "\tmov eax, [rel vint_%s]\n\timul eax, %s\n\tmov [rel vint_%s], eax ; Mul\n", ct[5], ct[6], ct[5]); }
void div(char* args) { split(args); fprintf(f, "\tmov eax, [rel vint_%s]\n\tcdq\n\tmov ecx, %s\n\tidiv ecx\n\tmov [rel vint_%s], eax ; Div\n", ct[5], ct[6], ct[5]); }

qaCmd cmds[] = {
    {"sm", startMain},
    {"em", endMain},

    {"step", step},
    {"es", es},
    {"call", call},

    {"prt", prt},
    {"prti", prti},
    {"prtln", prtln},

    {"add", add},
    {"sub", sub},
    {"mul", mul},
    {"div", div},
};
int cmd_count = sizeof(cmds) / sizeof(qaCmd);
